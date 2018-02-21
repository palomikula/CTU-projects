package cz.cvut.fel.pjv;
import java.util.HashMap;

public class BruteForceAttacker2 extends Thief {
    
    public char[] chars;
    public char[] possiblePassword;
    public HashMap charMap;
    public int n;
    public boolean end = false;
    
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
        if(n==0){
            for (int i = 0; i < sizeOfPassword; i++) {
                possiblePassword[i]='\0';
            }
            tryOpen(possiblePassword);
            return;
        }
        combs(0);
    }
    
    public void combs(int index){
        
        for (int i = 0; i < chars.length; i++) {
            if(index == n-1){
                if(tryOpen(possiblePassword) || isEnd()){
                    end = true;
                    return;
                }
                shift(index);
            }
            else{
                if(end){
                    return;
                }
                combs(index+1);
            }
        }
        
    }
    
    public void shift(int index){
        
        if((int)charMap.get(possiblePassword[index])+1 < chars.length){
            possiblePassword[index] = chars[(int)charMap.get(possiblePassword[index])+1];
            return;
        }
        
        while((int)charMap.get(possiblePassword[index])+1 == chars.length){
            possiblePassword[index] = chars[0];
            shift(index-1);
        }
        
    }
    
    public boolean isEnd(){
        boolean ret = true;
        for (int i = 0; i < n; i++) {
            ret = ret && (possiblePassword[i] == chars[chars.length -1]);
        }
        return ret;
    }
    
}