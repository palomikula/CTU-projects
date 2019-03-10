package cz.cvut.fel.agents.pdv.exclusion;

import cz.cvut.fel.agents.pdv.clocked.ClockedMessage;
import cz.cvut.fel.agents.pdv.clocked.ClockedProcess;
import cz.cvut.fel.agents.pdv.dsand.Pair;

import java.util.*;

class PairTimeComp implements Comparator<Pair<String,Integer>>{
    @Override
    public int compare(Pair<String,Integer> p1, Pair<String,Integer> p2){
        if(!p1.getSecond().equals(p2.getSecond()))
            return p1.getSecond().compareTo(p2.getSecond());
        else
            return p1.getFirst().compareTo(p2.getFirst());
    }

}

public class ExclusionPrimitive {

    /**
     * Stavy, ve kterych se zamek muze nachazet.
     */
    enum AcquisitionState {
        RELEASED,    // Uvolneny   - Proces, ktery vlastni aktualni instanci ExclusionPrimitive o pristup do kriticke
                     //              sekce nezada

        WANTED,      // Chteny     - Proces, ktery vlastni aktualni instanci ExclusionPrimitive zada o pristup do
                     //              kriticke sekce. Ten mu ale zatim nebyl odsouhlasen ostatnimi procesy.

        HELD         // Vlastneny  - Procesu bylo prideleno pravo pristupovat ke sdilenemu zdroji.
    }

    private ClockedProcess owner;            // Proces, ktery vlastni aktualni instanci ExclusionPrimitive

    private String criticalSectionName;      // Nazev kriticke sekce. POZOR! V aplikaci se muze nachazet vice kritickych
                                             // sekci s ruznymi nazvy!

    private String[] allAccessingProcesses;  // Seznam vsech procesu, ktere mohou chtit pristupovat ke kriticke sekci
                                             // s nazvem 'criticalSectionName' (a tak spolurozhoduji o udelovani pristupu)

    private AcquisitionState state;          // Aktualni stav zamku (vzhledem k procesu 'owner').
                                             // state==HELD znamena, ze proces 'owner' muze vstoupit do kriticke sekce

    // Doplnte pripadne vlastni datove struktury potrebne pro implementaci
    // Ricart-Agrawalova algoritmu pro vzajemne vylouceni
    HashMap<String,PriorityQueue<Pair<String,Integer>>> waitingProcesses;
    HashMap<String,HashSet<String>> waitingForProcesses;
    //PriorityQueue<Triplet<String,String,Integer>> waitingProcesses;
    //PriorityQueue<Triplet<String,String,Integer>> waitingForProcesses;
    int myWantedTime;


    public ExclusionPrimitive(ClockedProcess owner, String criticalSectionName, String[] allProcesses) {
        this.owner = owner;
        this.criticalSectionName = criticalSectionName;
        this.allAccessingProcesses = allProcesses;
        this.waitingForProcesses = new HashMap<>();
        this.waitingProcesses = new HashMap<>();


        // Na zacatku je zamek uvolneny
        this.state = AcquisitionState.RELEASED;
    }

    /**
     * Metoda pro zpracovani nove prichozi zpravy
     *
     * @param m    prichozi zprava
     * @return 'true', pokud je zprava 'm' relevantni pro aktualni kritickou sekci.
     */
    public boolean accept(ClockedMessage m) {

        // Implementujte zpracovani prijimane zpravy informujici
        // o pristupech ke sdilenemu zdroji podle Ricart-Agrawalova
        // algoritmu. Pokud potrebujete posilat specificke zpravy,
        // vytvorte si pro ne vlastni tridy.
        //
        // POZOR! Ne vsechny zpravy, ktere v teto metode dostanete Vas
        // budou zajimat! Budou Vam prichazet i zpravy, ktere se  napriklad
        // tykaji jinych kritickych sekci. Pokud je zprava nerelevantni, tak
        // ji nezpracovavejte a vratte navratovou hodnotu 'false'. Nekdo jiny
        // se o ni urcite postara :-)
        //
        // Nezapomente se starat o cas procesu 'owner'
        // pomoci metody owner.increaseTime(). Aktualizaci
        // logickeho casu procesu s prijatou zpravou (pomoci maxima) jsme
        // za Vas jiz vyresili.
        //
        // Cas poslani prijate zpravy muzete zjistit dotazem na hodnotu
        // m.sentOn. Aktualni logicky cas muzete zjistit metodou owner.getTime().
        // Zpravy se posilaji pomoci owner.send(prijemce, zprava) a je jim auto-
        // maticky pridelen logicky cas odeslani. Retezec identifikujici proces
        // 'owner' je ulozeny v owner.id.



        if (m instanceof ReqMessage) {
            //owner.increaseTime();
            if (!((ReqMessage) m).criticalSectionName.equals(criticalSectionName)) return false;
            if(state == AcquisitionState.RELEASED || (state == AcquisitionState.WANTED && (m.sentOn < myWantedTime || (m.sentOn == myWantedTime && owner.id.compareTo(m.sender) > 0)))){
                owner.increaseTime();
                owner.send(m.sender, new PermitMessage(criticalSectionName));
            }else{
                PriorityQueue<Pair<String,Integer>> tmp = waitingProcesses.get(criticalSectionName);
                if (tmp == null)
                    tmp = new PriorityQueue<>(new PairTimeComp());
                tmp.add(new Pair<>(m.sender,m.sentOn));
                waitingProcesses.put(criticalSectionName,tmp);

            }
            return true;
        }else if (m instanceof PermitMessage){
            owner.increaseTime();
            if (!((PermitMessage) m).criticalSectionName.equals(criticalSectionName)) return false;
            if(!waitingForProcesses.containsKey(criticalSectionName))return true;
            if(!waitingForProcesses.get(criticalSectionName).remove(m.sender)) return true;
            if (waitingForProcesses.get(criticalSectionName).isEmpty()) state = AcquisitionState.HELD;
            return true;
        }
        //if (!waitingForProcesses.containsKey(criticalSectionName) || waitingForProcesses.get(criticalSectionName).isEmpty()) state = AcquisitionState.HELD;

        //throw new RuntimeException("Not implemented yet");
        return false;
    }

    public void requestEnter() {

        owner.increaseTime();


        if (AcquisitionState.RELEASED != state) {
            // owner.increaseTime();
            return;
        }
        System.out.println(owner.id + " wants " + criticalSectionName);
        myWantedTime = owner.getTime();
        //System.out.println("a".compareTo("b"));
        state = AcquisitionState.WANTED;
        if (!waitingForProcesses.containsKey(criticalSectionName)){
            HashSet<String> tmp = new HashSet<>();
            waitingForProcesses.put(criticalSectionName, tmp);
        }
        //owner.increaseTime();
        int size = 0;
        for(String process : allAccessingProcesses){
            if(process.equals(owner.id))continue;
            size++;
            waitingForProcesses.get(criticalSectionName).add(process);
            owner.send(process,new ReqMessage(criticalSectionName));
        }
        if(size==0) state = AcquisitionState.HELD;


        //throw new RuntimeException("Not implemented yet");

    }

    public void exit() {

        // Implementuje uvolneni zdroje, aby k nemu meli pristup i
        // ostatni procesy z 'allAccessingProcesses', ktere ke zdroji
        // mohou chtit pristupovat
        //if(!waitingProcesses.containsKey(criticalSectionName))return;
        owner.increaseTime();
        while (waitingProcesses.containsKey(criticalSectionName)){
            Pair<String,Integer> tmp = waitingProcesses.get(criticalSectionName).poll();
            if(tmp==null)break;
            owner.send(tmp.getFirst(),new PermitMessage(criticalSectionName));
        }
        /*for (String process : allAccessingProcesses){
            if(owner.id.equals(process))continue;
            owner.send(process,new PermitMessage(criticalSectionName));
        }*/
        state = AcquisitionState.RELEASED;



        //throw new RuntimeException("Not implemented yet");

    }

    public String getName() {
        return criticalSectionName;
    }

    public boolean isHeld() {
        return state == AcquisitionState.HELD;
    }

}
