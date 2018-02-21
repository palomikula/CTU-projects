/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author palo
 */
public enum Notes {
    C2 (65.4F, "C2"), Cs2 (69.3F, "Cs2"), D2 (73.4F, "D2"),
    Ds2 (77.8F, "Ds2"), E2 (82.4F, "E2"), F2 (87.3F, "F2"),
    Fs2 (92.5F, "Fs2"), G2 (98.0F, "G2"), Gs2 (103.8F, "Gs2"),
    A2 (110F, "A2"), As2 (116.5F, "As2"), B2 (123.5F, "B2"),
    C3 (130.8F, "C3"), Cs3 (138.6F, "Cs3"), D3 (146.8F, "D3"),
    Ds3 (155.6F, "Ds3"), E3 (164.8F, "E3"), F3 (174.6F, "F3"),
    Fs3 (185.0F, "Fs3"), G3 (196.0F, "G3"), Gs3 (207.7F, "Gs3"),
    A3 (220.0F, "A3"), As3 (233.1F, "As3"), B3 (246.9F, "B3"),
    C4 (261.6F, "C4"), Cs4 (277.2F, "Cs4"), D4 (293.7F, "D4"),
    Ds4 (311.1F, "Ds4"), E4 (329.6F, "E4"), F4 (349.2F, "F4");
    
    final float frequency;
    final String name;

    Notes(float frequency, String name){
        this.frequency = frequency;
        this.name = name;
    }
}
