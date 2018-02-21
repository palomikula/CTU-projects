
package cz.cvut.fel.pjv;
import cz.cvut.fel.pjv.CircularArrayQueue;

/**
 * @author Jirka Šebek
 */
public class Start {

    public static void main(String[] args) {

         
        /* homework */
        CircularArrayQueue queue = new CircularArrayQueue(4);
        System.out.println("size: " + queue.size());
        queue.enqueue("Starkiller");
        
        System.out.println("size: " + queue.size());
        queue.enqueue("C-3PO");
        System.out.println("size: " + queue.size());
        queue.enqueue("Jabba the Hutt");
        System.out.println("size: " + queue.size());
        queue.enqueue("HK-47");
        System.out.println("size: " + queue.size());
        queue.enqueue("Darth Nihilus");
        System.out.println("size: " + queue.size());
        queue.enqueue("Count Dooku");
        System.out.println("size: " + queue.size());
        System.out.println("Value dequeued from CircularArrayQueue: " + queue.dequeue());
        System.out.println("printing all elements: ");
        queue.printAllElements();
        queue.enqueue("Jango Fett");
        System.out.println("size: " + queue.size()+"\n");
        System.out.print(queue.arr[queue.front-1]);
    }
}
