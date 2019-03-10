package cz.cvut.fel.agents.pdv.exclusion;

import java.util.Objects;

public class Triplet<T,U,V> {
    T first;
    U second;
    V third;

    public Triplet(T first, U second, V third) {
        this.first = first;
        this.second = second;
        this.third = third;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Triplet<?, ?, ?> triplet = (Triplet<?, ?, ?>) o;
        return Objects.equals(first, triplet.first) && Objects.equals(third, triplet.third) && Objects.equals(second, triplet.second);
    }

    @Override
    public int hashCode() {

        return Objects.hash(first, second, third);
    }

    public T getFirst() {
        return first;
    }

    public U getSecond() {
        return second;
    }

    public V getThird() { return third; }

}
