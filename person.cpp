#include <iostream>
#include <string>
using namespace std;

class Personne {
protected:
    int id;
    string prenom;
    string nom;
    string dateNaissance;
    string sexe;

    string email;
    string telephone;
    string adresse;

    bool actif;

public:
    // Constructeur par défaut
    Personne()
        : id(0), actif(true) {}

    // Constructeur avec paramètres
    Personne(
        int id,
        const string& prenom,
        const string& nom,
        const string& dateNaissance,
        const string& email
    )
        : id(id),
          prenom(prenom),
          nom(nom),
          dateNaissance(dateNaissance),
          email(email),
          actif(true) {}

    virtual ~Personne() {}

    // Getters
    int getId() const {
        return id;
    }

    string getPrenom() const {
        return prenom;
    }

    string getNom() const {
        return nom;
    }

    string getNomComplet() const {
        return prenom + " " + nom;
    }

    string getEmail() const {
        return email;
    }

    bool estActif() const {
        return actif;
    }

    // Setters
    void setEmail(const string& email) {
        this->email = email;
    }

    void setTelephone(const string& telephone) {
        this->telephone = telephone;
    }

    void setAdresse(const string& adresse) {
        this->adresse = adresse;
    }

    void setActif(bool statut) {
        actif = statut;
    }

    
    virtual void afficherInfos() const {
        cout << "ID : " << id << endl;
        cout << "Nom complet : " << getNomComplet() << endl;
        cout << "Email : " << email << endl;
        cout << "Actif : " << (actif ? "Oui" : "Non") << endl;
    }
};

class MenuManager {
public:
    static void showMainMenu() {
        cout << "=================================\n";
        cout << "   SCHOOL MANAGEMENT SYSTEM\n";
        cout << "=================================\n";
        cout << "1. Login\n";
        cout << "2. Exit\n";
        cout << "Choose an option: ";
    }

    static void showLoginMenu() {
        string username, password;

        cout << "\n--- Login ---\n";
        cout << "Username: ";
        cin >> username;

        cout << "Password: ";
        cin >> password;

        // TEMP logic
        if (username == "admin" && password == "1234") {
            cout << "Welcome Director!\n";
        } else {
            cout << "Invalid credentials.\n";
        }
    }
};

int main() {
    int choice;

    do {
        MenuManager::showMainMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                MenuManager::showLoginMenu();
                break;
            case 2:
                cout << "Goodbye!\n";
                break;
            default:
                cout << "Invalid option.\n";
        }
    } while (choice != 2);

    return 0;
}
