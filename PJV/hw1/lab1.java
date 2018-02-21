/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author palo
 */

import java.util.Scanner;

public class Lab01 {
   Scanner sc = new Scanner(System.in);
   
   public void start(String[] args) {
     homework();
   }
   
   public void homework() {
     System.out.print("Vyber operaci (1-soucet, 2-rozdil, 3-soucin, 4-podil:\n");
     int volba = sc.nextInt();
     
     switch(volba) {
       case 1:
         soucet();
         break;
       case 2:
         rozdil();
         break;
       case 3:
         soucin();
         break;
       case 4:
         podil();
         break;
       default:
         System.out.println("Chybna volba!\n");
     }
   }
   
   public void soucet() {
     System.out.print("Zadej scitanec: \n");
     double scitanec1 = sc.nextDouble();
     
     System.out.print("Zadej scitanec: \n");
     double scitanec2 = sc.nextDouble();
     
     System.out.print("Zadej pocet desetinnych mist: \n");
     int desetiny = sc.nextInt();
     
     if(desetiny < 0) {
       System.out.println("Chyba - musi byt zadane kladne cislo!\n");
       return;
     }
     
     double vysledek = scitanec1 + scitanec2;
     
     String scitanec1Str = String.format(java.util.Locale.US,"%." + desetiny + "f", scitanec1);
     String scitanec2Str = String.format(java.util.Locale.US,"%." + desetiny + "f", scitanec2);
     String vysledekStr = String.format(java.util.Locale.US,"%." + desetiny + "f", vysledek);
     
     System.out.println(scitanec1Str + " + " + scitanec2Str + " = " + vysledekStr);
   }
   
   public void rozdil() {
     System.out.print("Zadej mensenec: \n");
     double mensenec = sc.nextDouble();
     
     System.out.print("Zadej mensitel: \n");
     double mensitel = sc.nextDouble();
     
     System.out.print("Zadej pocet desetinnych mist: \n");
     int desetiny = sc.nextInt();
     
     if(desetiny < 0) {
       System.out.println("Chyba - musi byt zadane kladne cislo!\n");
       return;
     }
     
     double vysledek = mensenec - mensitel;
     
     String mensenecStr = String.format(java.util.Locale.US,"%." + desetiny + "f", mensenec);
     String mensitelStr = String.format(java.util.Locale.US,"%." + desetiny + "f", mensitel);
     String vysledekStr = String.format(java.util.Locale.US,"%." + desetiny + "f", vysledek);
     
     System.out.println(mensenecStr + " - " + mensitelStr + " = " + vysledekStr);
   }
   
   public void soucin() {
     System.out.print("Zadej cinitel: \n");
     double cinitel1 = sc.nextDouble();
     
     System.out.print("Zadej cinitel: \n");
     double cinitel2 = sc.nextDouble();
     
     System.out.print("Zadej pocet desetinnych mist: \n");
     int desetiny = sc.nextInt();
     
     if(desetiny < 0) {
       System.out.println("Chyba - musi byt zadane kladne cislo!\n");
       return;
     }
     
     double vysledek = cinitel1 * cinitel2;
     
     String cinitel1Str = String.format(java.util.Locale.US,"%." + desetiny + "f", cinitel1);
     String cinitel2Str = String.format(java.util.Locale.US,"%." + desetiny + "f", cinitel2);
     String vysledekStr = String.format(java.util.Locale.US,"%." + desetiny + "f", vysledek);
     
     System.out.println(cinitel1Str + " * " + cinitel2Str + " = " + vysledekStr);
   }
   
   public void podil() {
     System.out.print("Zadej delenec: \n");
     double delenec = sc.nextDouble();
     
     System.out.print("Zadej delitel: \n");
     double delitel = sc.nextDouble();
     
     if(delitel == 0) {
       System.out.println("Pokus o deleni nulou!\n");
       return;
     }
     
     System.out.print("Zadej pocet desetinnych mist: \n");
     int desetiny = sc.nextInt();
     
     if(desetiny < 0) {
       System.out.println("Chyba - musi byt zadane kladne cislo!\n");
       return;
     }
     
     double vysledek = delenec / delitel;
     
     String delenecStr = String.format(java.util.Locale.US,"%." + desetiny + "f", delenec);
     String delitelStr = String.format(java.util.Locale.US,"%." + desetiny + "f", delitel);
     String vysledekStr = String.format(java.util.Locale.US,"%." + desetiny + "f", vysledek);
     
     System.out.println(delenecStr + " / " + delitelStr + " = " + vysledekStr);
   }
}
