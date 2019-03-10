package cz.cvut.fel.agents.pdv.student;

import cz.cvut.fel.agents.pdv.dsand.Pair;
import cz.cvut.fel.agents.pdv.raft.messages.ClientRequestWithContent;

public class ConfirmRepMessage extends MessageWithTerm {

    Pair<ClientRequestWithContent,Integer> entry;

    ConfirmRepMessage(int term, Pair<ClientRequestWithContent,Integer> entry){
        super(term);
        this.entry = entry;
    }
}
