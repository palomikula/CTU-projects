
import java.util.Collections;
import java.util.Arrays;


/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author palo
 */

public class AisTransformer extends Thread{
    float[] arr;
    static int length;
    public Complex[] data;
    int sampleRate = 44100;
    int size;
    int L = 4; //Number of harmonics
    
    //desired frequency range
    float maxPitch = 350F;
    float minPitch = 65F;
            
    public AisTransformer(float[] arr) {
        this.arr = arr;
        this.length = arr.length;
        
    }
    
    /**
     * returns next power of two
     * 
     * @param old_length
     * @return 
     */
    private int nextPowerOf2(int old_length)
    {
        int new_length = 1;
        while (new_length < old_length){
            new_length = new_length << 1;
        }

        this.size = new_length;
        return new_length;
    }
    
    //
    /**
     * transforms the data to Complex[] and zeropads to the next power of 2
     * 
     * @param arr
     * @return 
     */
    private Complex[] adjust(float[] arr){
        int size = nextPowerOf2(this.length);
        data = new Complex[size];
        for (int i = 0; i < size; i++) {
            data[i] =  new Complex((i<this.length) ? arr[i]: 0, 0);
        }
        return data;
    }
    
    
    /**
     * compute the FFT of array, assuming its length is a power of 2
     * 
     * @param x
     * @return 
     */
    private Complex[] fft(Complex[] x) {
        int n = x.length;

        if (n == 1) return new Complex[] { x[0] };

        
        if (n % 2 != 0) { throw new RuntimeException("n is not a power of 2"); }
        

        // fft of even terms
        Complex[] even = new Complex[n/2];
        for (int k = 0; k < n/2; k++) {
            even[k] = x[2*k];
        }
        Complex[] q = fft(even);

        // fft of odd terms
        Complex[] odd  = even;
        for (int k = 0; k < n/2; k++) {
            odd[k] = x[2*k + 1];
        }
        Complex[] r = fft(odd);

        // combine
        Complex[] y = new Complex[n];
        for (int k = 0; k < n/2; k++) {
            float kth = (float)(-2 * k * Math.PI / n);
            Complex wk = new Complex((float)Math.cos(kth), (float)Math.sin(kth));
            y[k]       = q[k].plus(wk.times(r[k]));
            y[k + n/2] = q[k].minus(wk.times(r[k]));
        }
        
        return y;
    }
    /**
     * gets rid of garbage frequencies at the end by deleting the second half
     * 
     * @param y
     * @return 
     */
    //
    private Complex[] fft_useful(Complex[] y){
        int n = y.length;
        Complex[] newy = new Complex[n/2];
        newy = Arrays.copyOfRange(y, 0, n/2);
        return newy;
    }

    
    /**
     * this method calculates pitch by using harmonic summation to eliminate
     * octave shift and maps all the frequencies to the desired range to increase
     * accuracy of the estimated pitch
     * 
     * @param frequencies
     * @return 
     */
    private float calculatePitch(float[] frequencies){
        float curFreq = minPitch*2;
        int i = 0;
        int harmonicIndex;
        float[] harmonicSums = new float[10*((int)(maxPitch-minPitch)*2)+1];
        int pitchIndex;
        float pitch;
        while(curFreq<=maxPitch*2){
            harmonicSums[i] = 0;
            for (int l = 1; l <= L; l++) {
                harmonicIndex = (int)Math.round(curFreq*l*(0.5*frequencies.length/sampleRate));
                harmonicSums[i] += frequencies[harmonicIndex];
            }            
            curFreq+=0.1;
            i++;
        }
        System.out.println();
        pitchIndex = maxIndex(harmonicSums);
        pitch = minPitch*2 + (((float)pitchIndex)/10);
        return pitch/2;
    }
    /**
     * 
     * @param array
     * @return 
     */
    private int maxIndex(float[] array){
        int maxIndex = 0;
        float maxValue = array[0];
        for (int i = 0; i < array.length; i++) {
            if(maxValue<array[i]){
                maxValue = array[i];
                maxIndex = i;
            }
        }
        return maxIndex;
    }

    @Override
    public void run(){
        Complex[] data = this.adjust(arr);
        Complex[] afterFFT = new Complex[data.length];
        float[] frequencies = new float[data.length];
        afterFFT = fft_useful(fft(data));
        for (int i = 0; i < afterFFT.length; i++) {
            frequencies[i] = ((afterFFT[i].abs())*(afterFFT[i].abs()));
        }
        
        if (Buttons.noteToTune == null) {
            Tuner tuner = new Tuner(calculatePitch(frequencies));
        }
        
        else{
            Tuner tuner = new Tuner(calculatePitch(frequencies),Buttons.noteToTune);
        }
        
                
    }
}
