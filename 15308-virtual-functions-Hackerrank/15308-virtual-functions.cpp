

class Person {
public:
    string name;
    int age;

    virtual void getdata() = 0;
    virtual void putdata() = 0;
};

class Professor : public Person {
private:
    int publications;
    int cur_id;
    static int id;

public:
    Professor() {
        cur_id = ++id;
    }

    void getdata() {
        cin >> name >> age >> publications;
    }

    void putdata() {
        cout << name << " " << age << " " << publications << " " << cur_id << endl;
    }
};

int Professor::id = 0;

class Student : public Person {
private:
    int marks[6];
    int cur_id;
    static int id;

public:
    Student() {
        cur_id = ++id;
    }

    void getdata() {
        cin >> name >> age;
        for (int i = 0; i < 6; i++)
            cin >> marks[i];
    }

    void putdata() {
        int sum = 0;
        for (int i = 0; i < 6; i++)
            sum += marks[i];

        cout << name << " " << age << " " << sum << " " << cur_id << endl;
    }
};

int Student::id = 0;


// Synced seamlessly with LeetHub Pro
// Pro features: https://bit.ly/leethubpro | Free version: https://bit.ly/leethubv4
// Get it here: https://chromewebstore.google.com/detail/bcilpkkbokcopmabingnndookdogmbna