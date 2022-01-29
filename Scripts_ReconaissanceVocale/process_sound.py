###
# @author : Arnaud HINCELIN
# @date : 10 dec 2021
# @objective : creat a supervised model for keywords recognition
# @area : Graduation project - ESEO - 2021/2022
###




import os
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '3' 

import numpy as np
import matplotlib.pyplot as plt
#import tensorflow as tf
from tensorflow import cast, abs, float32, newaxis
from tensorflow.keras.models import Sequential
from sklearn.model_selection import train_test_split
from tensorflow.python.keras.layers import Dense, Dropout, Conv2D, MaxPooling2D, Resizing, Input, Flatten
from tensorflow.signal import stft 
from tensorflow.io import read_file
from tensorflow.audio import decode_wav
from tensorflow import squeeze
from tensorflow.keras.losses import SparseCategoricalCrossentropy
from tensorflow.keras.optimizers import Adam
from tqdm import tqdm

# file = 16Bits
# 


def read_wav_file(file):
    binary_file = read_file(filename=file) #file of type String with hexadecimal number of 16bits
    audio, samplerate = decode_wav( contents=binary_file ) # normalize audio data inot float between (0 - 1) 
    samplerate = np.array( samplerate )
    audio = squeeze(audio, axis=-1) #enlève dernière colonne 
    return (audio, samplerate)


"""
#OP plot
fig, axes = plt.subplots(nrows=1, ncols=1);
axes.plot( audio.numpy() )
axes.set_yticks( np.arange(-1.2, 1.2, 0.2) )
axes.set_title("avance")
"""
def plot_signal(audio, samplerate, label, num):
    length = audio.shape[0] / samplerate
    time = np.linspace(0., length, audio.shape[0])
    plt.figure()
    plt.plot(time, audio, label="mono channel")
    plt.legend()
    plt.xlabel("Time [s]")
    plt.ylabel("Amplitude")
    plt.show()


def create_dataset(directory, categories):
    dataset = []
    X = []
    y = []

    for category in tqdm(categories):
        path = os.path.join(directory, category)
        class_number = categories.index(category)
        for audio in tqdm( os.listdir(path=path) ):
            audio_wav,_ = read_wav_file( file=os.path.join(path, audio) )
            spectrogram = transform_into_spectrogram( waveform=audio_wav )
            dataset.append( [spectrogram, class_number] )
    
    for features, label in tqdm(dataset) : 
        X.append(features)
        y.append(label)
    X = np.array( X )
    y = np.array( y )

    print(X[0].shape)
    
    #NORMALISATION -> 3 NORM dispos : MinMax - Standardisation (moyenne) - RobustScaler (mediane) 
    #deja realisee

    (X_train, X_test, y_train, y_test) = train_test_split(X, y, test_size=0.1, shuffle=True)

    return (X_train, X_test, y_train, y_test)

"""
Wavesforms are represented in the time-domain, we have to transform into the time-frequency-domain (different from frequency-domain).
To perform that, we don't use FFT because it converts signal to components frequencies but loses all time information.
We are going to use the STFT (Short-Time-Fourier-Transform), to convert waveforms to spectograms which show frequency changes over the time.
Like 2D images, so the NN will be a CNN with spectrograms images inputed.

More about STFT : splits the signal into windows of time and runs a Fourier transform on each window, preserving some time information, \
    and returning a 2D tensor that you can run standard convolutions on.
"""

def transform_into_spectrogram(waveform):
    frame_length = 255 
    frame_step = 128
    #all waveforms have to have the same lentgh ; if needed use the zero padding (tf.zeros)
    # - 
    #cast into f32
    waveform = cast(x=waveform, dtype=float32)
    #convert into spectogram with STFT, which produces tab with complex values
    #spectogram = tf.signal.stft(signals=waveform, frame_length=frame_length, frame_step=frame_step)
    #spectogram = stft(signals=waveform, frame_length=frame_length, frame_step=frame_step)

    Nwindows = waveform.shape[0] // frame_length
    arr = np.reshape( waveform[:Nwindows * frame_length], (-1, frame_length) )
    spectogram = np.fft.rfft(arr, n=128)

    #obtain the magnitude and remove complex values, possible with abs (absolute value)
    spectogram = abs(x=spectogram)
    # spectogram.shape = (62, 65)
    #add a last channel to create a 3D image-like input in the CNN
    spectogram = spectogram[..., newaxis]
    #(62, 65, 1)

    return spectogram

def plot_spectogram(waveform, spectrogram, label, num):
    fig, axes = plt.subplots(2, figsize=(12,8))
    title_fig = label+" n° "+ str(num)
    fig.suptitle(title_fig, fontsize=14, fontweight='bold')
    timescale = np.arange(waveform.shape[0])
    axes[0].plot(timescale, waveform)
    axes[0].set_title('Waveform')
    axes[0].set_xlim([0, 16000])

    #to plot, ensure that spectrogram has only 2 dimesions
    if len(spectrogram.shape)>2:
        spectrogram = np.squeeze(a=spectrogram, axis=-1)
    #convert frequencies to log scale and we transpose ; finnally the time will be represented on the x-axis (colums# )
    log_spec = np.log(spectrogram.T + np.finfo(float).eps )
    height = log_spec.shape[0]
    width = log_spec.shape[1]
    X = np.linspace(0, np.size(spectrogram), num=width, dtype=int)
    Y = range(height)

    axes[1].pcolormesh(X, Y, log_spec, shading='auto', cmap='Spectral')
    axes[1].set_title('Spectogram')
    plt.show()


"""
-> Couche Dropout(rate) : 
Note : Couche seulement acive durant le training.
à chaque étape du training : 
- Pose 1 input à 0, de manière aléatoire et avec un taux (rate)
- Autres inputs sont scaled par (1/(1-rate)) ; ainsi la somme de tous les inputs initiaux reste inchangée.
Cela sert à éviter l'overfitting (=sur-apprentissage).

-> Couche de preprocessing : Resizing(height, width)
Resize une image input en un output de dimension height*width
Input est un array/tensor 4D (batched) ou 3D (unbatched)


"""

def create_model(input_shape, output_shape):
    model = Sequential()
    model.add( Input(shape=input_shape) )
    model.add( Resizing(height=32, width=32) )
    model.add( Conv2D(filters=32, kernel_size=3, activation='relu') )
    model.add( Conv2D(filters=64, kernel_size=3, activation='relu') )
    model.add( MaxPooling2D() )
    model.add( Dropout(rate=0.25) )
    model.add( Flatten() )
    model.add( Dense(units=128, activation='relu') )
    model.add( Dropout(rate=0.5) )
    model.add( Dense(units=output_shape, activation='softmax') )

    model.summary()
    # 1,625,221 parameters

    model.compile(
        optimizer = Adam(), 
        loss = SparseCategoricalCrossentropy(), 
        metrics=['accuracy'])

    return model

def train_model(model, X, y):
    EPOCHS = 10
    history = model.fit(
        x=X, 
        y=y, 
	batch_size=64, 
	epochs=EPOCHS, 
	validation_split=0.2
        )
    return history

def test_model(model, X, y):
    (val_loss, val_accuracy) = model.evaluate(X, y)
    print("\nCOST : ", val_loss)
    print("ACCURACY : ", val_accuracy)

def plot_model_loss(history):
    plt.figure('LOSS')
    plt.plot(history.history['loss'])
    plt.plot(history.history['val_loss'])
    plt.xlabel('epochs')
    plt.ylabel('loss')
    plt.legend(['loss', 'val_loss'])

    plt.figure('ACC')
    plt.plot(history.history['accuracy'])
    plt.plot(history.history['val_accuracy'])
    plt.xlabel('epochs')
    plt.ylabel('accuracy')
    plt.legend(['accuracy', 'val_accuracy'])

    plt.show()



###---------------------------------------------------------------------------------------------------------------------------------------------
### MAIN ----------------------------------------------------------------------------------------------------------------------------------------
###----------------------------------------------------------------------------------------------------------------------------------------------

PATH_DIR = "audio_data"
CATEGORIES = ["avance", "droite", "gauche", "recule", "stop"]
K = 5



PLOT = 56


(X_train, X_test, y_train, y_test) = create_dataset(PATH_DIR, CATEGORIES)
print("shape of X_train : ", X_train.shape)
print("shape of X_train[0] : ", X_train[0].shape)


if 0:
	spec = transform_into_spectrogram( waveform=X_train[ PLOT ] )
	plot_spectogram(waveform=X_train[ PLOT ] ,spectrogram=spec, label= CATEGORIES[y_train[ PLOT ]], num=PLOT )

model = create_model(X_train[0].shape, K)
print("input : ", X_train[0])
history = train_model(model, X=X_train, y=y_train)
model.save("model_audio_reco_v1.h5")
test_model(model, X=X_test, y=y_test)
plot_model_loss(history=history)












