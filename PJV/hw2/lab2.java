/*
 * File name: Lab06.java
 * Date:      2014/08/26 21:39
 * Author:    @author
 */

package cz.cvut.fel.pjv;

import java.util.Scanner;

public class Lab02 {

    public void start(String[] args) {
        double[] arr = new double[10];
        double average;
        double std_dev;
        double sum = 0;
        double dev_sum = 0;
        int i;
        int l = 0;
        Scanner sc = new Scanner(System.in);
        for (i=0; sc.hasNext(); i++) {
            l++;
            while(!sc.hasNextDouble()){
                sc.nextLine();
                sc.nextLine();
                System.out.printf("A number has not been parsed from line %d!\n",l);
                l++;
            }
            arr[i] = sc.nextDouble();
            sum += arr[i];
            if(i==9){
                average = sum/10;
                sum = 0;
                for (int j = 0; j < 10; j++) {
                    dev_sum += (arr[j]-average)*(arr[j]-average);  
                    i=-1;
                }
                std_dev = Math.sqrt(dev_sum/10);
                System.out.printf("10 %.3f %.3f\n",average, std_dev); 
                dev_sum = 0;
            }
        }
        System.out.print("End of input detected!\n");
        
        if(i>1){
            average = sum/i;
                for (int j = 0; j < i; j++) {
                    dev_sum += (arr[j]-average)*(arr[j]-average);  
                }
            std_dev = Math.sqrt(dev_sum/i);
            System.out.printf("%2d %.3f %.3f\n",i, average, std_dev);            
        }                
    }
}

/* end of Lab02.java */
