/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
import javax.sound.sampled.*;
import java.io.*;
import java.util.logging.Level;
import java.util.logging.Logger;
/**
 *
 * @author palo
 */
public class AudioRecorder{

 
    // record duration, in milliseconds
    //static final long RECORD_TIME = 1000;  // 
 
 
    // format of audio file
    AudioFileFormat.Type fileType = AudioFileFormat.Type.WAVE;
 
    // the line from which audio data is captured
    TargetDataLine line;
    byte[] data = new byte[10000000];
    /**
     * Defines an audio format
     */
    AudioFormat getAudioFormat() {
        float sampleRate = 44100;
        int sampleSizeInBits = 16;
        int channels = 1;
        boolean signed = true;
        boolean bigEndian = true;
        
        AudioFormat format = new AudioFormat(sampleRate, sampleSizeInBits,
                                             channels, signed, bigEndian);
        return format;
    }

    /**
     * records audio for specified time
     */
    public void record() {
        try {
            AudioFormat format = getAudioFormat();
            DataLine.Info info = new DataLine.Info(TargetDataLine.class, format);
 
            // checks if system supports the data line
            if (!AudioSystem.isLineSupported(info)) {
                System.out.println("Line not supported");
                System.exit(0);
            }
            line = (TargetDataLine) AudioSystem.getLine(info);
            line.open(format);
            line.start();   // start capturing

            AudioInputStream ais = new AudioInputStream(line);
            long length;
            length = ais.read(data);
            float[] newData = new float[Math.round(length/2)];
            DataInputStream dis = new DataInputStream(new ByteArrayInputStream(data));
            
            //transforms audio data into float values between -1 and 1 
            for (int i = 0; i < length; i+=2) {
                 newData[i/2] =((data[i] << 8) | (data[i + 1] & 0xFF) )/32768.0F;
            }
            
            //gives the data to a new thread that calculates pitch
            Thread fft = new AisTransformer(newData);
            fft.start();

        } catch (LineUnavailableException ex) {
            Logger.getLogger(AudioRecorder.class.getName()).log(Level.SEVERE, null, ex);
        } catch (IOException ioe) {
            Logger.getLogger(AudioRecorder.class.getName()).log(Level.SEVERE, null, ioe);
        }
    }
    
     /**
      * Closes the target data line to finish capturing and recording
      */
    void finish() {
            
        line.stop();
        line.close();
       
    }
   
}

