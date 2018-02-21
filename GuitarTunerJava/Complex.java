/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author palo
 */
public class Complex {
    private float re;   // the real part
    private float im;   // the imaginary part

    /**
     * create a new object with the given real and imaginary parts
     * 
     * @param real
     * @param imag 
     */

    public Complex(float real, float imag) {
        this.re = real;
        this.im = imag;
    }
    
    /**
     * returns absolute value
     * 
     * @return 
     */
    public float abs() {
        return (float)(Math.hypot(re, im));
    }
    
    /**
     * returns a new Complex object with value (this + a)
     * 
     * @param a
     * @return 
     */
    
    public Complex plus(Complex a){
        float real = this.re + a.re;
        float imag = this.im + a.im;
        return new Complex(real, imag);
    }

    /**
     * return a new Complex object with value (this - a)
     * 
     * @param a
     * @return 
     */
    public Complex minus(Complex a) {
        float real = this.re - a.re;
        float imag = this.im - a.im;
        return new Complex(real, imag);
    }
    
    /**
     * return a new Complex object with value (this * a)
     * 
     * @param a
     * @return 
     */
    public Complex times(Complex a) {
        float real = this.re * a.re - this.im * a.im;
        float imag = this.re * a.im + this.im * a.re;
        return new Complex(real, imag);
    }
    
}
