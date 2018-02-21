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
public class TreeImpl implements Tree {
    public TreeImpl() {
    }
    
    int values[];
    Node root;
    
    public Node setNodes(int first, int last){
        if (first == last){
            NodeImpl node = new NodeImpl(this.values[first],null,null);
            return node;
        }
        else{
        int prnt = ((last-first+1)/2)+first;
        int nodeValue = this.values[prnt];
        NodeImpl node = new NodeImpl(nodeValue,setNodes(first, prnt-1),(last >= prnt+1) ? setNodes(prnt + 1, last):null);
        return node;
        }
    }
    
	public void setTree(int[] values){
            this.values = values;
            this.root = (values.length > 0) ? setNodes(0, values.length - 1) : null;
        }
	
	/**
	 * @return root of the tree, or null if the tree is empty.
	 */
        
	public Node getRoot(){
            return this.root;
        }
	
	/**
	 * Tree description looks as follows (for tree created from values [1, 2, 3, 4, 5, 6, 7]):
	 * - 4
	 *  - 2
	 *   - 1
	 *   - 3
	 *  - 6
	 *   - 5
	 *   - 7
	 * 
	 * @return String representation of the tree, or "" for empty tree
	 */
        @Override
	public String toString(){
            String string = "";
            if(this.root == null){
                return string;
            }
            else{
                string += "- " + Integer.toString(this.root.getValue()) + "\n";
                if(this.root.getLeft() != null) 
                    string += toStringRecursive(this.root.getLeft(), 1);
                if(this.root.getRight() != null) 
                    string += toStringRecursive(this.root.getRight(), 1);
            }
            return string;
        }
        
        private String toStringRecursive(Node n, int depth){
            String string = "";
            for (int i = 0; i < depth; i++) {
                string += " ";
            }
            string += "- " + Integer.toString(n.getValue()) + "\n";
            if(n.getLeft() != null){
                string += toStringRecursive(n.getLeft(), depth+1);
            }
            if(n.getRight() != null){
                string += toStringRecursive(n.getRight(), depth+1);
                }
            return string;
        }
}