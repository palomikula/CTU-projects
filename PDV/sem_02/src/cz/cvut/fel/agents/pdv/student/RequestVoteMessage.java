package cz.cvut.fel.agents.pdv.student;

import cz.cvut.fel.agents.pdv.student.MessageWithTerm;

public class RequestVoteMessage extends MessageWithTerm {
    RequestVoteMessage(int term){
        super(term);
    }
}
