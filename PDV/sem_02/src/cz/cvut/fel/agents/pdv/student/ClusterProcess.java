package cz.cvut.fel.agents.pdv.student;

import cz.cvut.fel.agents.pdv.dsand.Message;
import cz.cvut.fel.agents.pdv.dsand.Pair;
import cz.cvut.fel.agents.pdv.raft.RaftProcess;
import cz.cvut.fel.agents.pdv.raft.messages.ClientRequestWhoIsLeader;
import cz.cvut.fel.agents.pdv.raft.messages.ClientRequestWithContent;
import cz.cvut.fel.agents.pdv.raft.messages.ServerResponseConfirm;
import cz.cvut.fel.agents.pdv.raft.messages.ServerResponseLeader;

import java.util.*;
import java.util.function.BiConsumer;

/**
 * Vasim ukolem bude naimplementovat (pravdepodobne nejenom) tuto tridu. Procesy v clusteru pracuji
 * s logy, kde kazdy zanam ma podobu mapy - kazdy zaznam v logu by mel reprezentovat stav
 * distribuovane databaze v danem okamziku.
 *
 * Vasi implementaci budeme testovat v ruznych scenarich (viz evaluation.RaftRun a oficialni
 * zadani). Nasim cilem je, abyste zvladli implementovat jednoduchou distribuovanou key/value
 * databazi s garancemi podle RAFT.
 */
public class ClusterProcess extends RaftProcess<Map<String, String>> {

    // ostatni procesy v clusteru
    private final List<String> otherProcessesInCluster;
    // maximalni spozdeni v siti
    private final int networkDelays;
    private int term;
    private boolean voted;
    enum ServerState {
        LEADER,
        FOLLOWER,
        CANDIDATE
    }
    ServerState state;
    private int clock;
    private String leader;
    private int myVotes;
    private int confirms;
    public Map<String,String> database;
    private ArrayList<Pair<ClientRequestWithContent,Integer>> log;
    int lastIndex = -1;
    private Map<String,Pair<String,Integer>> waitingClients;
    private Set<String> processedRequests;

    public ClusterProcess(String id, Queue<Message> inbox, BiConsumer<String, Message> outbox,
                          List<String> otherProcessesInCluster, int networkDelays) {
        super(id, inbox, outbox);
        this.otherProcessesInCluster = otherProcessesInCluster;
        this.networkDelays = 1;
        this.term = 0;
        this.clock = 0;
        state = ServerState.FOLLOWER;
        voted = false;
        leader = null;
        myVotes = 0;
        //confirms = 0;
        database = new HashMap<>();
        log = new ArrayList<>();
        waitingClients = new HashMap<>();
        processedRequests = new HashSet<>();

    }

    @Override
    public Optional<Map<String, String>> getLastSnapshotOfLog() {
        String finalString1 = "";
        String finalString2 = "";
        for (Pair<ClientRequestWithContent,Integer> entry : log){
            Pair p = (Pair) entry.getFirst().getContent();
            finalString1 = "" + p.getFirst();
            finalString2 += p.getSecond();
        }
        database.put(finalString1,finalString2);
        //System.out.println(finalString1+ " " + finalString2);
        // komentar viz deklarace
        return Optional.of(database);
    }

    @Override
    public String getCurrentLeader() {

        // komentar viz deklarace
        return leader;
    }

    @Override
    public void act() {

        // doimplementuje metodu act() podle RAFT

        // krome vlastnich zprav (vasich trid), dostavate typy zprav z balicku raft.messages s rodicem
        // ClientRequest, tak si je projdete, at vite, co je ucelem a obsahem jednotlivych typu.
        // V pripade ClientRequestWithContent dostavate zpravu typu
        // ClientRequestWithContent<StoreOperationEnums, Pair<String, String>>, kde StoreOperationEnums
        // je operace, kterou mate udelat s obsahem paru Pair<String, String>, kde prvni hodnota
        // paru je klic (nikdy neni prazdny) a druha je hodnota (v pripade pozadavku GET je prazdny)

        // dejte si pozor na jednotlive akce podle RAFT. S klientem komunikujte vyhradne pomoci zprav
        // typu ServerResponse v messages

        // na pozadavky klientu odpovidate zpravami typu ServerResponse viz popis podtypu v messages.
        // !!! V TOMTO PRIPADE JE 'requestId' ROVNO POZADAVKU KLIENTA, NA KTERY ODPOVIDATE !!!

        // dalsi podrobnosti naleznete na strance se zadanim

        if(state == ServerState.LEADER){
            for(String process : otherProcessesInCluster){
                send(process, new HeartbeatMessage(term));
            }
        }

        while(!inbox.isEmpty()){
            Message m = inbox.poll();
            if (m instanceof MessageWithTerm){
                //term = Math.max(term, ((MessageWithTerm) m).term);
                if(((MessageWithTerm) m).term < term) continue;
                if(m instanceof HeartbeatMessage){
                    if(!m.sender.equals(getCurrentLeader())){
                        //System.err.println("unexpected leader");
                        //clock = 0;
                    }else {
                        clock = 0;
                    }
                }
                if(m instanceof RequestVoteMessage){
                    if ((!voted) && term < ((RequestVoteMessage) m).term){
                        term = ((MessageWithTerm) m).term;
                        voted = true;
                        //leader = null;
                        state = ServerState.FOLLOWER;
                        clock = 0;
                        myVotes = 0;
                        send(m.sender, new VoteMessage(term));
                    }
                    continue;
                }
                if (m instanceof VoteMessage){
                    myVotes++;
                    if(myVotes > otherProcessesInCluster.size() + 1 - myVotes){
                        for (String process : otherProcessesInCluster){
                            send(process, new IAmLeaderMessage(term));
                        }
                        leader = getId();
                        state = ServerState.LEADER;
                        voted = false;
                        myVotes = 0;
                        clock = 0;
                    }
                    continue;
                }
                if(m instanceof IAmLeaderMessage){
                    leader = m.sender;
                    voted = false;
                    myVotes = 0;
                    state = ServerState.FOLLOWER;
                    clock = 0;
                    continue;
                }
                if(m instanceof LogRepMessage){
                    int lastIndex = log.size()-1;
                    int lastTerm = (lastIndex >= 0) ? log.get(lastIndex).getSecond() : -1;
                    //System.out.println("lastIndex = "+lastIndex + "\nlastTerm = "+lastTerm);
                    if(/*m.sender.equals(leader) && */!processedRequests.contains(((LogRepMessage)m).entry.getFirst().getRequestId())
                            /*  && ((LogRepMessage)m).lastIndex == lastIndex && ((LogRepMessage)m).lastTerm == lastTerm*/) {
                        processedRequests.add(((LogRepMessage)m).entry.getFirst().getRequestId());
                        log.add(((LogRepMessage) m).entry);
                        send(m.sender,new ConfirmRepMessage(term,((LogRepMessage) m).entry));
                    }
                    clock = 0;
                    continue;
                }
                if(m instanceof ConfirmRepMessage){
                    String tmp1 = waitingClients.get(((ConfirmRepMessage)m).entry.getFirst().getRequestId()).getFirst();
                    int tmp2 = waitingClients.get(((ConfirmRepMessage)m).entry.getFirst().getRequestId()).getSecond() + 1;
                    if(tmp2 > otherProcessesInCluster.size() + 1 - confirms ){
                        //System.out.println("yea");
                        send(tmp1,new ServerResponseConfirm(((ConfirmRepMessage)m).entry.getFirst().getRequestId()));
                    }
                    waitingClients.put(((ConfirmRepMessage)m).entry.getFirst().getRequestId(),new Pair<>(tmp1,tmp2));
                }
            }


            if(m instanceof ClientRequestWhoIsLeader){
                send(m.sender, new ServerResponseLeader(((ClientRequestWhoIsLeader) m).getRequestId(),leader));
            }else if (m instanceof ClientRequestWithContent){
                if(((ClientRequestWithContent) m).getOperation().getName().equals("APPEND")) {
                    if (processedRequests.contains(((ClientRequestWithContent) m).getRequestId())){
                        send(m.sender, new ServerResponseConfirm(((ClientRequestWithContent) m).getRequestId()));
                        continue;
                    }
                    waitingClients.put(((ClientRequestWithContent) m).getRequestId(), new Pair<>(m.sender,0));
                    processedRequests.add(((ClientRequestWithContent)m).getRequestId());
                    int lastIndex = log.size()-1;
                    int lastTerm = (lastIndex >= 0) ? log.get(lastIndex).getSecond() : -1;
                    for(String process : otherProcessesInCluster){
                        send(process,new LogRepMessage(term,new Pair<>((ClientRequestWithContent) m,new Integer(term)),lastIndex,lastTerm));
                    }
                    log.add(new Pair<>((ClientRequestWithContent) m,new Integer(term)));

                }
            }

        }

        if ((state == ServerState.FOLLOWER && clock > networkDelays + 1 + (int)(Math.random() * 10)) || (state == ServerState.CANDIDATE && clock > (networkDelays ) + 3 + (int)(Math.random() * 10))){
            term++;
            voted = true;
            state = ServerState.CANDIDATE;
            myVotes = 1;
            clock = 0;
            leader = null;
           // System.out.println(getId() + " kandiduje za leadera, term " + term);
            for(String process : otherProcessesInCluster){
                send(process, new RequestVoteMessage(term));
            }
            //System.out.println();
        }

        clock++;

    }
}
