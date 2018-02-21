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

public class Scale extends JPanel{

    Graphics g;
    float diff;

    public Scale(float diff) {
        this.diff = diff;
    }
    
    
    /**
     * Creates new graphic component that tells user if
     * the current note is correctly tuned
     * 
     * @param g 
     */
    @Override
    public void paintComponent(Graphics g) {
        super.paintComponent(g);
        Rectangle area = this.getBounds();
        int X,Y;
        X = (int)area.getCenterX() - WIDTH/2;
        Y = (int)area.getCenterY() - HEIGHT/2;
        if (diff == GUI.initValue) {
            new Strip(g, X-13, Y, 23);
        }
        else if(Math.abs(diff)<0.2F){
            new Strip(g, X-13, Y, 23, Color.GREEN);
        }else{
            new Strip(g, X-13, Y, 23, Color.RED);
        }
        
        for (int i = 1; i <= 20; i++) {
            if (diff!=GUI.initValue && i*0.1 <= diff-0.1) {
                new Strip(g,X+i*13,Y,10,Color.RED);
            }else{
                new Strip(g,X+i*13,Y,10);
            }
        }
        
        for (int i = 2; i <= 21; i++) {
            
            if ((i-1)*-0.1 >= diff+0.1) {
                new Strip(g,X-i*13,Y,10,Color.RED);
            }else{
                new Strip(g,X-i*13,Y,10);
            }
        }
         
        
    }
   
}
