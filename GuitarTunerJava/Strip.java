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

public class Strip {
    public int X;
    public int Y;
    Graphics g;
    public Color color;
    public int WIDTH;
    public int HEIGHT;

    /**
     * Creates one empty element of the scale graphic component
     * 
     * @param g
     * @param X
     * @param Y
     * @param WIDTH 
     */
    public Strip(Graphics g, int X, int Y, int WIDTH) {
        this.g = g;
        this.X = X;
        this.Y = Y;
        this.WIDTH = WIDTH;
        this.HEIGHT = 30;
        drawEmptyStrip();
    }
    
    /**
     * Creates one element of the scale graphic component with passed color
     * 
     * @param g
     * @param X
     * @param Y
     * @param WIDTH
     * @param color 
     */
    public Strip(Graphics g, int X, int Y, int WIDTH, Color color) {
        this.g = g;
        this.X = X;
        this.Y = Y;
        this.color = color;
        this.WIDTH = WIDTH;
        this.HEIGHT = 30;
        drawStrip();
    }    
 
    
    public void drawEmptyStrip(){
        g.setColor(Color.LIGHT_GRAY);
        g.fillRoundRect(X, Y, WIDTH, HEIGHT, 10, 10);
        g.setColor(Color.black);
        g.drawRoundRect(X, Y, WIDTH, HEIGHT, 10, 10);
    }
    
    public void drawStrip(){
        g.setColor(color);
        g.fillRoundRect(X, Y, WIDTH, HEIGHT, 10, 10);
        g.setColor(Color.BLACK);
        g.drawRoundRect(X, Y, WIDTH, HEIGHT, 10, 10);
    }
}
