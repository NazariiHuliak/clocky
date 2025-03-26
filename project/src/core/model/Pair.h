#ifndef PAIR_H
#define PAIR_H

template <typename K, typename V>
struct Pair {
    K key;
    V value;

    Pair(K k, V v) : key(k), value(v) {}

    String toString() const {
        return String(key) + " -> " + String(value);
    }
};

#endif
