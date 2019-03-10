package cz.cvut.fel.agents.pdv.swim;

import cz.cvut.fel.agents.pdv.dsand.Message;
import cz.cvut.fel.agents.pdv.dsand.Pair;

import java.util.*;

/**
 * Trida s implementaci metody act() pro proces Failure Detector. Tuto tridu (a tridy pouzivanych zprav) budete
 * odevzdavat. Do tridy si muzete doplnit vlastni pomocne datove struktury. Hodnoty muzete inicializovat primo
 * v konstruktoru. Klicova je metoda act(), kterou vola kazda instance tridy FailureDetectorProcess ve sve metode
 * act(). Tuto metodu naimplementujte podle protokolu SWIM predstaveneho na prednasce.
 *
 * Pokud si stale jeste nevite rady s frameworkem, inspiraci muzete nalezt v resenych prikladech ze cviceni.
 */
public class ActStrategy {

    // maximalni zpozdeni zprav
    private final int maxDelayForMessages;
    private final int period;
    private final List<String> otherProcesses;
    private final Set<String> deadProcesses;
    Random rand;
   // int k = 500;

    // Definujte vsechny sve promenne a datove struktury, ktere budete potrebovat
    private int wakeCount = 0;
    private Map<String, Integer> pingedProcesses;
    private Map<String, Pair<String,Integer>> reqPingedProcesses;
    private Map<String, Pair<String,Integer>> helperProcesses;

    public ActStrategy(int maxDelayForMessages, List<String> otherProcesses,
                       int timeToDetectKilledProcess, int upperBoundOnMessages) {
        this.maxDelayForMessages = maxDelayForMessages;
        this.period = 10;
        this.otherProcesses = otherProcesses;
        this.pingedProcesses = new HashMap<>();
        this.reqPingedProcesses = new HashMap<>();
        this.deadProcesses  = new HashSet<>();
        this.rand = new Random();

        // Doplne inicializaci

    }

    /**
     * Metoda je volana s kazdym zavolanim metody act v FailureDetectorProcess. Metodu implementujte
     * tak, jako byste implementovali metodu act() v FailureDetectorProcess, misto pouzivani send()
     * pridejte zpravy v podobe paru - prijemce, zprava do listu. Zpravy budou nasledne odeslany.
     * <p>
     * Diky zavedeni teto metody muzeme kontrolovat pocet odeslanych zprav vasi implementaci.
     */
    public List<Pair<String, Message>> act(Queue<Message> inbox, String disseminationProcess) {
        List<Pair<String, Message>> toSend = new LinkedList();


        // Od DisseminationProcess muzete dostat zpravu typu DeadProcessMessage, ktera Vas
        // informuje o spravne detekovanem ukoncenem procesu.
        // DisseminationProcess muzete poslat zpravu o detekovanem "mrtvem" procesu.
        // Zprava musi byt typu PFDMessage.
        
        while(!inbox.isEmpty()){
            Message message = inbox.poll();
            
            if(message instanceof AckMessage){
                pingedProcesses.remove(message.sender);
                if (reqPingedProcesses.containsKey(message.sender)){
                    ((LinkedList<Pair<String,Message>>) toSend).addLast(new Pair<>(reqPingedProcesses.get(message.sender).getFirst(),new AckReqMessage(message.sender)));
                    reqPingedProcesses.remove(message.sender);
                    //System.out.println("got ack from  " + message.sender );
                }
                //System.out.println("got ack from  " + message.sender );
            }

            if(message instanceof AckReqMessage){
            pingedProcesses.remove(((AckReqMessage) message).getProcessToAck());
            }

            if(message instanceof PingMessage){
                ((LinkedList<Pair<String,Message>>) toSend).addLast(new Pair<>(message.sender, new AckMessage()));
                //System.out.println(message.recipient + "  sending ack to  " + message.sender);
            }

            if(message instanceof PingReqMessage){
                ((LinkedList<Pair<String,Message>>) toSend).addLast(new Pair<>(((PingReqMessage) message).getProcessToPing(), new PingMessage()));
                reqPingedProcesses.put(((PingReqMessage) message).getProcessToPing(),new Pair<>(message.sender, wakeCount));
            }

        }


        for (Map.Entry<String, Integer> process : pingedProcesses.entrySet()){
            if(!deadProcesses.contains(process.getKey())) {
                if (wakeCount > (process.getValue() + (2 * (3) * maxDelayForMessages))) {
                    //System.out.println("process "+process.getKey()+":\n"+"wakecount = " + wakeCount +"\n" + "processWakeCount = " + process.getValue()+"\n\n");
                    ((LinkedList<Pair<String, Message>>) toSend).addLast(new Pair<>(disseminationProcess, new PFDMessage(process.getKey())));
                    deadProcesses.add(process.getKey());
                    otherProcesses.remove(process.getKey());
                    //System.out.println("found dead process " + process.getKey());
                }else if((wakeCount-process.getValue()) % (maxDelayForMessages) == 0){
                    String rndProcess = otherProcesses.get(rand.nextInt(otherProcesses.size()));
                    //helperProcesses.put(rndProcess,new Pair<>(process.getKey(),wakeCount));
                    ((LinkedList<Pair<String, Message>>) toSend).addLast(new Pair<>(rndProcess, new PingReqMessage(process.getKey())));
                }
            }
        }

        if (wakeCount % period == 0) {
            String rndProcess = otherProcesses.get(rand.nextInt(otherProcesses.size()));
            pingedProcesses.put(rndProcess,wakeCount);
            ((LinkedList<Pair<String, Message>>) toSend).addLast(new Pair<>(rndProcess, new PingMessage()));
            //System.out.println("sending ping to " + rndProcess);
        }
        



        wakeCount++;
        return toSend;
    }

}
