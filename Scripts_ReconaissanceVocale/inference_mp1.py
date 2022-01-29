###
# @author : Arnaud HINCELIN
# @date : 17 jan 2022
# @objective : inference for keywords recognition
# @area : Graduation project - ESEO - 2021/2022
###


import numpy as np
import tflite_runtime.interpreter as tflr
import loader
import time

def read_audio_wav_file(path_file):
        audio_file,_ = loader.audio_from_file(path_file)
        print("shape audio : ", audio_file.shape )
        
        return audio_file
        
"""
Why STFT
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
    # - 
    #cast into f32
    #waveform = np.cast(x=waveform, dtype='float32')
    waveform = np.cast['float32'](waveform)
    #convert into spectogram with STFT, which produces tab with complex values
    spectogram = run_stft(waveform, frame_length)
    
    #obtain the magnitude and remove complex values, possible with abs (absolute value)
    spectogram = np.absolute(spectogram)
    # spectogram.shape = (124, 129)
    
    #add a last channel to create a 3D image-like input in the CNN
    spectogram = spectogram[..., np.newaxis]
    # spectogram.shape = (124, 129, 1)
    
    print("spectrogram shape : ", spectogram.shape)
    
    return spectogram


def run_stft(input_wave, fen_len):
    #Short-time Fourier transform: convert a 1D vector to a 2D array
    Nwindows = input_wave.shape[0] // fen_len
    arr = np.reshape( input_wave[:Nwindows * fen_len], (-1, fen_len) )
    spectogram = np.fft.rfft(arr, n=128)
    return spectogram

def process_inference(spectrogram_input, interpreter):

    #reshape
    spectrogram_input = spectrogram_input.reshape( -1, 62, 65, 1 )

    interpreter.allocate_tensors() #before execution
    # Get input and output tensors.
    input_details = interpreter.get_input_details()
    output_details = interpreter.get_output_details()

    # Test the model on random input data.
    input_shape = input_details[0]['shape']
    input_data = np.array( spectrogram_input, dtype=np.float32)
    interpreter.set_tensor(input_details[0]['index'], input_data)

    interpreter.invoke()

    vector_predicted = interpreter.get_tensor(output_details[0]['index'])
    
    proba_max = np.max(vector_predicted)
    label_predicted = np.argmax(vector_predicted)

    return (label_predicted, proba_max)


def save_result_outpuNN(result):
    with open('voice_reco_predict.txt', 'w') as f:
        f.write( str(result) )



###      ###
### MAIN ###
###      ###

CATEGORIES = ["avance", "droite", "gauche", "recule", "stop"]

interpreter = tflr.Interpreter(model_path="model_audio_reco_TFLITE.tflite" )


audio = read_audio_wav_file("command_record.wav")

start = time.time()

spectrogram = transform_into_spectrogram(audio)
(label_predicted, proba_max) = process_inference( spectrogram_input=spectrogram, interpreter=interpreter )

end = time.time()


print("\n -- VOICE RECOGNITION --")
print("predicted command : ", CATEGORIES[label_predicted], "|probability : ", proba_max, "\n")
print("Inference duration : {:.4f}".format(end-start), "seconds")
print("\n -- END --")

save_result_outpuNN(label_predicted)


