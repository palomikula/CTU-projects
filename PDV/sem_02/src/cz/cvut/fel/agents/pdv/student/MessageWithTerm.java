package cz.cvut.fel.agents.pdv.student;

import cz.cvut.fel.agents.pdv.dsand.Message;

public abstract class MessageWithTerm extends Message {
    int term;

    public MessageWithTerm (int term){
        this.term = term;
    }
}
