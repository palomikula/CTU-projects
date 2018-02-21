/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author palo
 */

import java.awt.*; 
import java.awt.event.*;
import javax.swing.*;

public class Buttons extends JPanel implements ActionListener{
    JButton tuneAuto;
    JButton tuneManual;
    JButton stopTuning;
    JPanel panelTA;
    JPanel panelTM;
    JPanel stop;
    Choice notesSelect;
    Thread main; 
    public static Notes noteToTune;
    
    public Buttons(){
        
        setLayout(new GridLayout());
        
        panelTA = new JPanel(new GridBagLayout());
        tuneAuto = new JButton("Tune Auto");
        tuneAuto.addActionListener(this);
        panelTA.add(tuneAuto);
        add(panelTA);
        
        panelTM= new JPanel(new GridBagLayout());
        tuneManual = new JButton("Tune Manual");
        tuneManual.addActionListener(this);
        panelTM.add(tuneManual);
        
        notesSelect = new Choice();
        for (Notes note : Notes.values()) {
            notesSelect.add(note.name);
        }
        panelTM.add(notesSelect);
        add(panelTM);
        
        stop = new JPanel(new GridBagLayout());
        stopTuning = new JButton("Stop Tuning");
        stopTuning.addActionListener(this);
        stop.add(stopTuning);
        add(stop);
    }
    
    @Override
    public void actionPerformed(ActionEvent e){
        if (e.getSource() ==  tuneAuto && !GUI.running) {
            GUI.running = true;
            main = new Start();
            main.start();
        }
        if (e.getSource() ==  tuneManual && !GUI.running) {
            Buttons.noteToTune = Notes.valueOf(notesSelect.getSelectedItem());
            GUI.running = true;
            main = new Start();
            main.start();
        }
        if (e.getSource() ==  stopTuning && GUI.running) {
            Buttons.noteToTune = null;
            GUI.running = false;
        }
    }
}
