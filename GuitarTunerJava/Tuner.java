/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author palo
 */
public class Tuner {
    float pitch;
    public static float minDiff;
    Notes noteToTune;
    
    public Tuner(float pitch) {
        this.pitch = pitch;
        autoTune();
    }
    
    public Tuner(float pitch, Notes notetoTune){
        this.noteToTune = notetoTune;
        this.pitch = pitch;
        manualTune();
    }
    
    
    /**
     * finds out which note is the closest to estimated pitch and tells user
     * if it is the right frequency
     */
    public void autoTune(){
        
        minDiff = 1000F;
        for (Notes note : Notes.values()) {
            if(minDiff > Math.abs(pitch - note.frequency)){
                minDiff = pitch - note.frequency;
                noteToTune = note;
            }
        }
        GUI.scalePanel.remove(GUI.scale);
        GUI.scale = new Scale(minDiff);
        GUI.scalePanel.add(GUI.scale);

        if(GUI.running){
            //System.out.println(notesStrings[minDiffIndex] + "\n" + pitch + "\n" + ((minDiff<0.2F) ? "OK" : "not OK"));
            GUI.topLabel.setText("You are tuning note "+noteToTune.name + " (" + noteToTune.frequency + " Hz)");
            GUI.frequencyL.setText("Current pitch: " + String.valueOf(pitch) + "   " + ((Math.abs(minDiff)<0.2F) ? "OK" : "not OK"));
        }
    }
    
    public void manualTune(){
        GUI.scalePanel.remove(GUI.scale);
        GUI.scale = new Scale(pitch -noteToTune.frequency);
        GUI.scalePanel.add(GUI.scale);
        if(GUI.running){
            GUI.topLabel.setText("You are tuning note "+noteToTune.name + " (" + noteToTune.frequency + " Hz)");
            GUI.frequencyL.setText("Current pitch: " + String.valueOf(pitch) + "   " + ((Math.abs(pitch-noteToTune.frequency)<0.2F) ? "OK" : "not OK"));
        }
    }
    
    
}
