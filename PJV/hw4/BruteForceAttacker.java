package cz.cvut.fel.pjv;
import java.util.HashMap;

public class BruteForceAttacker extends Thief {
    
    public char[] chars;
    public char[] possiblePassword;
    public HashMap charMap;
    public int n;
    
    @Override
    public void breakPassword(int sizeOfPassword) {
        chars = this.getCharacters();
        charMap = new HashMap();
        for (int i = 0; i < chars.length; i++) {
            charMap.put(chars[i], i);
        }
        possiblePassword = new char[sizeOfPassword];
        for (int i = 0; i < sizeOfPassword; i++) {
            possiblePassword[i] =  chars[0];
        }
        n = sizeOfPassword;
        combs(n-1);
    }
    
    public void combs(int index){
        if(tryOpen(possiblePassword) || possiblePassword[0] == '\0'){
            return;
        }
        if(index == n-1 && possiblePassword[index]!='\0'){
            System.out.println(possiblePassword);
            System.out.println();
            shift(index);
            combs(index);
        }else if(possiblePassword[index] == '\0'){
            possiblePassword[index] = chars[0];
            shift(index-1);
            combs(index-1);
        }else{
            combs(n-1);
        }
        
    }
    
    public boolean shift(int index){
        if((int)charMap.get(possiblePassword[index])+1 == chars.length){
            possiblePassword[index] = '\0';
            return false;
        }
        else{
            possiblePassword[index] = chars[(int)charMap.get(possiblePassword[index])+1];
            return true;
        }
        
    }
    
}