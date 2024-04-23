#include "tree.h"


template <typename Key, typename Value>
class BPlusTree {
public:
    template <typename T>
    struct NewValue {
        int ttl;
        T value;
    };
    using new_value = NewValue<Value>;

    ~BPlusTree() = default;

    bool Set(const Key& key, const Value& value, int validity = 0) {
        return tree.insert(key, new_value{validity, value});
    };

    Value Get(const Key& key) const {
        const new_value& value = tree.search(key);
        return value.value;
    };

    bool Exists(const Key& key) const {
        return tree.Exists(key);
    };

    bool Del(const Key& key) {
        return tree.remove(key);
    };

    bool Update(const Key& key, const Value& value) {
        try {
            const new_value& old = tree.search(key);
            return tree.Update(key, new_value{old.ttl, value});
        } catch(const std::invalid_argument& err) {
            return false;
        }
    };

    // std::vector<Key> Keys() const {

    // };

    bool Rename(const Key& key, const Key& new_key) {
        const new_value& value = tree.search(key);
        bool res = tree.remove(key);
        if (res) {
            return tree.insert(key, new_value{0, value});
        }
    };

    // virtual int Ttl(const Key& param) const = 0;
    // virtual std::vector<Key> Find(const Value& value) const = 0;
    // virtual std::vector<Value> Showall() const = 0;
    // virtual std::pair<bool, int> Upload(const std::string& path) = 0;
    // virtual std::pair<bool, int> Export(const std::string& path) const = 0;
    void Print() {
        tree.printTree();
    }
private:
    Three<Key, new_value> tree;

};
