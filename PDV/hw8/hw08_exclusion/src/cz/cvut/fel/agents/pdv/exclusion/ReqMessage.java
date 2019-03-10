package cz.cvut.fel.agents.pdv.exclusion;

import cz.cvut.fel.agents.pdv.clocked.ClockedMessage;

public class ReqMessage extends ClockedMessage {

    String criticalSectionName;

    ReqMessage(String criticalSectionName){
        this.criticalSectionName = criticalSectionName;
    }
}
