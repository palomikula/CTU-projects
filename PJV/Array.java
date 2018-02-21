/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
import java.lang.Math;
import java.util.Scanner;
import java.util.Arrays;
/**
 *
 * @author palo
 */
public class Array {
    public void fill_array(int arr[]){
        int arr_size = arr.length;
        for (int i = 0; i < arr_size; i++) {
            arr[i] = i;
        }
    }
    public void fill_array_from_interval(double arr[], double min, double max){
        int arr_size = arr.length;
        double rnd = Math.random()*(max-min)+min; 
        for (int i = 0; i < arr_size; i++) {
            arr[i] = rnd;
        }
    }
    public void sort_array(double arr[]){
        Arrays.sort(arr);
    }
    
}
