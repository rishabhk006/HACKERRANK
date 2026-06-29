

class LRUCache : public Cache {
public:
    LRUCache(int capacity) {
        cp = capacity;
        head = tail = NULL;
    }

    void set(int key, int value) {
        if (mp.count(key)) {
            Node *cur = mp[key];
            cur->value = value;

            if (cur != head) {
                if (cur == tail) {
                    tail = tail->prev;
                    tail->next = NULL;
                } else {
                    cur->prev->next = cur->next;
                    cur->next->prev = cur->prev;
                }

                cur->prev = NULL;
                cur->next = head;
                head->prev = cur;
                head = cur;
            }
            return;
        }

        Node *node = new Node(key, value);

        if ((int)mp.size() == cp) {
            mp.erase(tail->key);

            if (head == tail) {
                delete tail;
                head = tail = NULL;
            } else {
                Node *temp = tail;
                tail = tail->prev;
                tail->next = NULL;
                delete temp;
            }
        }

        if (!head) {
            head = tail = node;
        } else {
            node->next = head;
            head->prev = node;
            head = node;
        }

        mp[key] = head;
    }

    int get(int key) {
        if (!mp.count(key))
            return -1;

        Node *cur = mp[key];

        if (cur != head) {
            if (cur == tail) {
                tail = tail->prev;
                tail->next = NULL;
            } else {
                cur->prev->next = cur->next;
                cur->next->prev = cur->prev;
            }

            cur->prev = NULL;
            cur->next = head;
            head->prev = cur;
            head = cur;
        }

        return cur->value;
    }
};



// Synced seamlessly with LeetHub Pro
// Pro features: https://bit.ly/leethubpro | Free version: https://bit.ly/leethubv4
// Get it here: https://chromewebstore.google.com/detail/bcilpkkbokcopmabingnndookdogmbna