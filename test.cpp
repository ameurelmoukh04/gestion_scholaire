#include <iostream>
#include <string>
using namespace std;

// ====================== CLASSE PERSONNE ======================
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
    Personne() : id(0), actif(true) {}

    Personne(int id, const string& prenom, const string& nom,
             const string& dateNaissance, const string& email)
        : id(id), prenom(prenom), nom(nom),
          dateNaissance(dateNaissance), email(email), actif(true) {}

    virtual ~Personne() {}

    string getNomComplet() const {
        return prenom + " " + nom;
    }

    virtual void afficherInfos() const {
        cout << "Nom : " << getNomComplet() << endl;
        cout << "Email : " << email << endl;
        cout << "Actif : " << (actif ? "Oui" : "Non") << endl;
    }

    virtual void menu() = 0;
};

// ====================== DIRECTEUR PEDAGOGIQUE ======================
class DirecteurPedagogique : public Personne {
public:
    DirecteurPedagogique()
        : Personne(1, "Admin", "Director", "01/01/1980", "admin@school.com") {}

    void menu() override {
        cout << "\n--- Menu Directeur Pédagogique ---\n";
        cout << "1. Gérer enseignants\n";
        cout << "2. Consulter rapports\n";
        cout << "3. Déconnexion\n";
    }
};

// ====================== ENSEIGNANT ======================
class Enseignant : public Personne {
public:
    Enseignant()
        : Personne(2, "Ali", "Teacher", "02/02/1990", "teacher@school.com") {}

    void menu() override {
        cout << "\n--- Menu Enseignant ---\n";
        cout << "1. Voir emploi du temps\n";
        cout << "2. Marquer absences\n";
        cout << "3. Déconnexion\n";
    }
};

// ====================== AGENT DE SECURITE ======================
class AgentSecurite : public Personne {
public:
    AgentSecurite()
        : Personne(3, "Omar", "Security", "03/03/1985", "security@school.com") {}

    void menu() override {
        cout << "\n--- Menu Agent de Sécurité ---\n";
        cout << "1. Contrôler accès\n";
        cout << "2. Signaler incident\n";
        cout << "3. Déconnexion\n";
    }
};

// ====================== MENU MANAGER ======================
class MenuManager {
public:
    static Personne* login() {
        string username, password;

        cout << "\n--- Login ---\n";
        cout << "Username: ";
        cin >> username;
        cout << "Password: ";
        cin >> password;

        if (username == "admin" && password == "1234") {
            cout << "Bienvenue Directeur\n";
            return new DirecteurPedagogique();
        }
        else if (username == "teacher" && password == "1234") {
            cout << "Bienvenue Enseignant\n";
            return new Enseignant();
        }
        else if (username == "security" && password == "1234") {
            cout << "Bienvenue Agent de Sécurité\n";
            return new AgentSecurite();
        }
        else {
            cout << "Identifiants invalides\n";
            return nullptr;
        }
    }
};

// ====================== MAIN ======================
int main() {
    int choix;
    Personne* user = nullptr;

    do {
        cout << "\n===== SCHOOL MANAGEMENT SYSTEM =====\n";
        cout << "1. Login\n";
        cout << "2. Exit\n";
        cout << "Choix : ";
        cin >> choix;

        if (choix == 1) {
            user = MenuManager::login();

            if (user != nullptr) {
                int action;
                do {
                    user->menu();
                    cout << "Choix : ";
                    cin >> action;
                } while (action != 3);

                delete user;
                user = nullptr;
            }
        }

    } while (choix != 2);

    cout << "Goodbye!\n";
    return 0;
}