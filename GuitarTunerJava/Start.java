/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author palo
 */

import java.util.logging.Level;
import java.util.logging.Logger;

public class Start extends Thread {
    
    static final long RECORD_TIME = 1000;

    @Override
    public void run() {
        while(GUI.running) {
    
            AudioRecorder recorder = new AudioRecorder();

            // creates a new thread that waits for a specified
            // time before stopping
            Thread stopper = new Thread(new Runnable() {
                public void run() {
                    try {
                        Thread.sleep(RECORD_TIME);
                    } catch (InterruptedException ex) {
                        Logger.getLogger(Start.class.getName()).log(Level.SEVERE, null, ex);
                    }
                    recorder.finish();
                }
            });

            stopper.start();

            // start recording
            recorder.record();
        }
    }
}
