package cz.cvut.fel.agents.pdv.swim;

import cz.cvut.fel.agents.pdv.dsand.Message;

public class AckReqMessage extends Message {

    private final String processToAck;

    public AckReqMessage(String processToAck) {
        this.processToAck = processToAck;
    }

    public String getProcessToAck() {
        return processToAck;
    }
}
