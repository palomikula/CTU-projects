package cz.cvut.fel.agents.pdv.exclusion;

import cz.cvut.fel.agents.pdv.clocked.ClockedMessage;

public class PermitMessage extends ClockedMessage {

    String criticalSectionName;

    PermitMessage(String criticalSectionName){
        this.criticalSectionName = criticalSectionName;
    }

}
