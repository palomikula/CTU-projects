/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package cz.cvut.fel.pjv;

/**
 *
 * @author palo
 */
public class NodeImpl implements Node{


    private int value;
    private Node lChild;
    private Node rChild;

    public NodeImpl(int value, Node lChild, Node rChild) {
        this.value = value;
        this.lChild = lChild;
        this.rChild = rChild;
    }
    
    @Override
    public Node getLeft() {
        return this.lChild;
    }

    @Override
    public Node getRight() {
        return this.rChild;
    }

    @Override
    public int getValue() {
        return this.value;
    }
    
}
