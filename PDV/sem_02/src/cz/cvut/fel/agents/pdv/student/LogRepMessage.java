package cz.cvut.fel.agents.pdv.student;

import cz.cvut.fel.agents.pdv.dsand.Pair;
import cz.cvut.fel.agents.pdv.raft.messages.ClientRequestWithContent;

public class LogRepMessage extends MessageWithTerm {
    Pair<ClientRequestWithContent,Integer> entry;
    int lastIndex;
    int lastTerm;

    public LogRepMessage (int term, Pair<ClientRequestWithContent,Integer> entry, int lastIndex, int lastTerm) {
        super(term);
        this.entry = entry;
        this.lastIndex = lastIndex;
        this.lastTerm = lastTerm;
    }
}
