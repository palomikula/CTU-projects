package cz.cvut.fel.agents.pdv.student;

import cz.cvut.fel.agents.pdv.student.MessageWithTerm;

public class HeartbeatMessage extends MessageWithTerm {
    HeartbeatMessage(int term){
        super(term);
    }
}
