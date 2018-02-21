package cz.cvut.fel.pjv;

/**
 * Implementation of the {@link Queue} backed by fixed size array.
 */
public class CircularArrayQueue implements Queue {
    int capacity;
    public int front;
    public int rear;
    public String[] arr;
    
    /**
     * Creates the queue with capacity set to the value of 5.
     */
    public CircularArrayQueue() {
        this.capacity = 5;
        this.front = -1;
        this.rear = -1;
        this.arr = new String[this.capacity];
    }


    /**
     * Creates the queue with given {@code capacity}. The capacity represents maximal number of elements that the
     * queue is able to store.
     * @param capacity of the queue
     */
    
    public CircularArrayQueue(int capacity) {
        this.capacity = capacity;
        this.front = -1;
        this.rear = -1;
        this.arr = new String[this.capacity];
    }

    @Override
    public int size() {
        if(this.isEmpty()){
            return 0;
        }
        if(this.front>this.rear){
            return (this.rear - this.front) + this.capacity+1;
        }
        return this.rear-this.front+1;
    }

    @Override
    public boolean isEmpty() {
        return (this.rear == -1 && this.front ==-1);
    }

    @Override
    public boolean isFull() {
        return ((this.rear+1)%this.capacity==this.front);
    }

    @Override
    public boolean enqueue(String obj) {
        if(obj == null || this.isFull()){
            return false;
        }
        if(this.isEmpty()){
            this.rear++;
            this.front++;
        }
        else{
            this.rear = (this.rear+1)%this.capacity;
        }
        this.arr[this.rear] = obj;
        
        return true;        
    }

    @Override
    public String dequeue() {
        if(this.isEmpty()){
            return null;
        }
        String ret;
        ret = this.arr[this.front];
        this.arr[this.front] = null;
        if(this.front == this.rear){
            this.rear = -1;
            this.front = -1;
            return ret;
        }
        this.front = (this.front+1)%this.capacity;
        return ret;
    }

    @Override
    public void printAllElements() {
        for (int i = 0; i <= this.size(); i++) {
            System.out.print(this.arr[(this.front+i)%this.capacity]+", i="+i+"\n");
            
        }
    }
}
