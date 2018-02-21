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

public class GUI extends JFrame{
    
    public Buttons buttons;
    public static JPanel topPanel;
    public static JLabel topLabel;
    public static JLabel frequencyL;
    public static boolean running = false;
    public static JPanel scalePanel;
    public static Scale scale;
    public static int initValue = 500;

    Rectangle bounds;

    public GUI() throws HeadlessException {
        super("GuitarTuner");
        this.setSize(720, 360); 
        this.setLocationRelativeTo(null);
        this.setLayout(new GridLayout(3,0));
               
        
        scalePanel = new JPanel(new BorderLayout());
        
        scale = new Scale(initValue);
        topPanel = new JPanel(new FlowLayout());
        topLabel = new JLabel();        
        topPanel.add(topLabel);
        
        scalePanel.add(topPanel,BorderLayout.PAGE_END);
        scalePanel.add(scale);
        add(scalePanel);
        
        JPanel panel2 = new JPanel(new GridBagLayout());
        GUI.frequencyL = new JLabel();
        panel2.add(frequencyL);
        add(panel2);
        
        buttons = new Buttons();
        add(buttons);        
        
        this.setVisible(true);
        //add(new JPanel());
        /*bounds = this.getBounds();
        System.out.println(bounds.toString());*/        
        
    } 
    
   
    public static void main(String[] args) {
        GUI gui = new GUI();
        gui.setDefaultCloseOperation(EXIT_ON_CLOSE);
       
    }
    
}
