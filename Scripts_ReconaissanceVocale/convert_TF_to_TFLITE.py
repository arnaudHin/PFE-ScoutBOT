###
# @author : Arnaud HINCELIN
# @date : 10 dec 2021
# @objective : creat a supervised model for keywords recognition
# @area : Graduation project - ESEO - 2021/2022
###

import tensorflow as tf
from tensorflow.keras.models import load_model


def convert_TFModel_to_TFLiteModel(tfModel):

    tfLiteModel = None
    converter = tf.lite.TFLiteConverter.from_keras_model(tfModel)
    tfLiteModel = converter.convert()

    file = open( 'model_audio_reco_TFLITE.tflite' , 'wb' ) 
    file.write( tfLiteModel )



###---------------------------------------------------------------------------------------------------------------------------------------------
### MAIN ----------------------------------------------------------------------------------------------------------------------------------------
###----------------------------------------------------------------------------------------------------------------------------------------------


mymodel = load_model("model_audio_reco_v1.h5")
convert_TFModel_to_TFLiteModel(tfModel=mymodel)



