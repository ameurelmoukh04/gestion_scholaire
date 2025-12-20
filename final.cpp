#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>

using namespace std;

// Forward declarations
class Etudiant;
class Enseignant;
class Caissiere;
class DirecteurPedagogique;

class Personne {
protected:
    int id;
    string prenom;
    string nom;
    string dateNaissance;
    string sexe;
    string email;
    string password;
    string telephone;
    string adresse;
    bool actif;
    int absence;
    int retard;

public:
    Personne()
        : id(0), actif(true), absence(0), retard(0) {}

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
          actif(true),
          absence(0),
          retard(0) {}

    virtual ~Personne() {}

    // Getters
    int getId() const { return id; }
    string getPrenom() const { return prenom; }
    string getNom() const { return nom; }
    string getNomComplet() const { return prenom + " " + nom; }
    string getEmail() const { return email; }
    string getPassword() const { return password; }
    string getDateNaissance() const { return dateNaissance; }
    string getTelephone() const { return telephone; }
    string getAdresse() const { return adresse; }
    bool estActif() const { return actif; }
    int getAbsence() const { return absence; }
    int getRetard() const { return retard; }

    // Setters
    void setEmail(const string& email) { this->email = email; }
    void setTelephone(const string& telephone) { this->telephone = telephone; }
    void setAdresse(const string& adresse) { this->adresse = adresse; }
    void setActif(bool statut) { actif = statut; }
    void setAbsence(int a) { absence = a; }
    void setRetard(int r) { retard = r; }

    // Absence / Retard
    void ajouterAbsence() { absence++; }
    void supprimerAbsence() { if (absence > 0) absence--; }
    void ajouterRetard() { retard++; }
    void supprimerRetard() { if (retard > 0) retard--; }

    // Affichage
    virtual void afficherInfos() const {
        cout << "ID : " << id << endl;
        cout << "Nom complet : " << getNomComplet() << endl;
        cout << "Date de naissance : " << dateNaissance << endl;
        cout << "Email : " << email << endl;
        cout << "Telephone : " << telephone << endl;
        cout << "Adresse : " << adresse << endl;
        cout << "Absences : " << absence << endl;
        cout << "Retards : " << retard << endl;
        cout << "Actif : " << (actif ? "Oui" : "Non") << endl;
    }

    // File I/O
    virtual string getType() const = 0;
    virtual string toFileString() const {
        stringstream ss;
        ss << id << ";" << prenom << ";" << nom << ";" << dateNaissance
           << ";" << email << ";" << telephone << ";" << adresse
           << ";" << actif << ";" << absence << ";" << retard;
        return ss.str();
    }

    virtual void fromFileString(const string& line) {
        stringstream ss(line);
        string item;
        getline(ss, item, ';'); id = stoi(item);
        getline(ss, prenom, ';');
        getline(ss, nom, ';');
        getline(ss, dateNaissance, ';');
        getline(ss, email, ';');
        getline(ss, password, ';');
        getline(ss, telephone, ';');
        getline(ss, adresse, ';');
        getline(ss, item, ';'); actif = (item == "1");
        getline(ss, item, ';'); absence = stoi(item);
        getline(ss, item, ';'); retard = stoi(item);
    }

    virtual void menu() = 0;
};

class Matiere {
    int id;
    string nom;
    int coefficient;
    string description;

public:
    Matiere(int i=0, string n="", int c=1, string d="")
        : id(i), nom(n), coefficient(c), description(d) {}

    int getId() const { return id; }
    string getNom() const { return nom; }
    int getCoefficient() const { return coefficient; }
    string getDescription() const { return description; }

    void modifier(string n, int c, string d) {
        nom = n;
        coefficient = c;
        description = d;
    }

    void afficher() const {
        cout << "Matiere [" << id << "] "
             << nom << " (coef " << coefficient << ") - "
             << description << endl;
    }

    string toFileString() const {
        stringstream ss;
        ss << id << ";" << nom << ";" << coefficient << ";" << description;
        return ss.str();
    }

    void fromFileString(const string& line) {
        stringstream ss(line);
        string item;
        getline(ss, item, ';'); id = stoi(item);
        getline(ss, nom, ';');
        getline(ss, item, ';'); coefficient = stoi(item);
        getline(ss, description, ';');
    }

    void sauvegarder() const {
        ofstream f("matieres.txt", ios::app);
        f << toFileString() << "\n";
        f.close();
    }
};

class Note {
    int id;
    float valeur;
    string date;
    int idMatiere;

public:
    Note(int i=0, float v=0, string d="", int idM=0)
        : id(i), valeur(v), date(d), idMatiere(idM) {}

    int getId() const { return id; }
    float getValeur() const { return valeur; }
    string getDate() const { return date; }
    int getIdMatiere() const { return idMatiere; }

    void modifier(float v) { valeur = v; }
    void setDate(const string& d) { date = d; }
    void setIdMatiere(int idM) { idMatiere = idM; }

    void afficher() const {
        cout << "Note [" << id << "] "
             << valeur << " | Date: " << date
             << " | Matiere ID: " << idMatiere << endl;
    }

    string toFileString(int idEtudiant) const {
        stringstream ss;
        ss << id << ";" << idEtudiant << ";" << idMatiere << ";"
           << valeur << ";" << date;
        return ss.str();
    }

    void fromFileString(const string& line, int& idEtudiant) {
        stringstream ss(line);
        string item;
        getline(ss, item, ';'); id = stoi(item);
        getline(ss, item, ';'); idEtudiant = stoi(item);
        getline(ss, item, ';'); idMatiere = stoi(item);
        getline(ss, item, ';'); valeur = stof(item);
        getline(ss, date, ';');
    }

    void sauvegarder(int idEtudiant) const {
        ofstream f("notes.txt", ios::app);
        f << toFileString(idEtudiant) << "\n";
        f.close();
    }
};

class Salle {
    int id;
    string numero;
    int capacite;
    int etage;
    bool disponible;

public:
    Salle(int i=0, string n="", int c=0, int e=0)
        : id(i), numero(n), capacite(c), etage(e), disponible(true) {}

    int getId() const { return id; }
    string getNumero() const { return numero; }
    int getCapacite() const { return capacite; }
    int getEtage() const { return etage; }
    bool estDisponible() const { return disponible; }

    void reserver() { disponible = false; }
    void liberer() { disponible = true; }
    void modifier(string n, int c, int e) {
        numero = n;
        capacite = c;
        etage = e;
    }

    void afficher() const {
        cout << "Salle [" << id << "] "
             << numero << " | Cap: " << capacite
             << " | Etage: " << etage
             << " | Etat: "
             << (disponible ? "Libre" : "Occupee") << endl;
    }

    string toFileString() const {
        stringstream ss;
        ss << id << ";" << numero << ";" << capacite << ";"
           << etage << ";" << disponible;
        return ss.str();
    }

    void fromFileString(const string& line) {
        stringstream ss(line);
        string item;
        getline(ss, item, ';'); id = stoi(item);
        getline(ss, numero, ';');
        getline(ss, item, ';'); capacite = stoi(item);
        getline(ss, item, ';'); etage = stoi(item);
        getline(ss, item, ';'); disponible = (item == "1");
    }

    void sauvegarder() const {
        ofstream f("salles.txt", ios::app);
        f << toFileString() << "\n";
        f.close();
    }
};

class Cours {
    int id;
    int idMatiere;
    int idSalle;
    string jour;
    string heureDebut;
    string heureFin;
    int idEtudiant;

public:
    Cours(int i=0, int idM=0, int idS=0, string j="", string hd="", string hf="", int idE=0)
        : id(i), idMatiere(idM), idSalle(idS), jour(j), heureDebut(hd), heureFin(hf), idEtudiant(idE) {}

    int getId() const { return id; }
    int getIdMatiere() const { return idMatiere; }
    int getIdSalle() const { return idSalle; }
    string getJour() const { return jour; }
    string getHeureDebut() const { return heureDebut; }
    string getHeureFin() const { return heureFin; }
    int getIdEtudiant() const { return idEtudiant; }

    void modifierHoraire(string hd, string hf) {
        heureDebut = hd;
        heureFin = hf;
    }

    void afficher() const {
        cout << "Cours [" << id << "] "
             << jour << " " << heureDebut << "-" << heureFin
             << " | Matiere ID: " << idMatiere
             << " | Salle ID: " << idSalle << endl;
    }

    string toFileString() const {
        stringstream ss;
        ss << id << ";" << idMatiere << ";" << idSalle << ";"
           << jour << ";" << heureDebut << ";" << heureFin << ";" << idEtudiant;
        return ss.str();
    }

    void fromFileString(const string& line) {
        stringstream ss(line);
        string item;
        getline(ss, item, ';'); id = stoi(item);
        getline(ss, item, ';'); idMatiere = stoi(item);
        getline(ss, item, ';'); idSalle = stoi(item);
        getline(ss, jour, ';');
        getline(ss, heureDebut, ';');
        getline(ss, heureFin, ';');
        if (ss.peek() != EOF) {
            getline(ss, item, ';'); idEtudiant = stoi(item);
        }
    }

    void sauvegarder() const {
        ofstream f("cours.txt", ios::app);
        f << toFileString() << "\n";
        f.close();
    }
};

class Emploi {
    int idEtudiant;
    vector<Cours> cours;

public:
    Emploi(int idE=0) : idEtudiant(idE) {}

    void ajouterCours(const Cours& c) {
        cours.push_back(c);
    }

    void supprimerCours(int index) {
        if (index >= 0 && index < (int)cours.size())
            cours.erase(cours.begin() + index);
    }

    vector<Cours>& getCours() { return cours; }
    const vector<Cours>& getCours() const { return cours; }

    void consulter() const {
        cout << "\n--- EMPLOI DU TEMPS ---\n";
        if (cours.empty()) {
            cout << "Aucun cours planifie.\n";
            return;
        }
        for (const auto& c : cours)
            c.afficher();
    }
};

class Paiement {
    int id;
    int idEtudiant;
    float montant;
    string date;
    string type;

public:
    Paiement(int i=0, int idE=0, float m=0, string d="", string t="")
        : id(i), idEtudiant(idE), montant(m), date(d), type(t) {}

    int getId() const { return id; }
    int getIdEtudiant() const { return idEtudiant; }
    float getMontant() const { return montant; }
    string getDate() const { return date; }
    string getType() const { return type; }

    void afficher() const {
        cout << "Paiement [" << id << "] "
             << montant << " DH | "
             << date << " | Type: " << type << endl;
    }

    string toFileString() const {
        stringstream ss;
        ss << id << ";" << idEtudiant << ";" << montant << ";" << date << ";" << type;
        return ss.str();
    }

    void fromFileString(const string& line) {
        stringstream ss(line);
        string item;
        getline(ss, item, ';'); id = stoi(item);
        getline(ss, item, ';'); idEtudiant = stoi(item);
        getline(ss, item, ';'); montant = stof(item);
        getline(ss, date, ';');
        getline(ss, type, ';');
    }

    void sauvegarder() const {
        ofstream f("paiements.txt", ios::app);
        f << toFileString() << "\n";
        f.close();
    }
};

// Data Manager Class
class DataManager {
public:
    static vector<Personne*> personnes;
    static vector<Matiere> matieres;
    static vector<Salle> salles;
    static vector<Cours> cours;
    static vector<Etudiant*> etudiants;
    static vector<Note> notes;
    static vector<Paiement> paiements;
    static int nextPersonneId;
    static int nextMatiereId;
    static int nextSalleId;
    static int nextCoursId;
    static int nextNoteId;
    static int nextPaiementId;

    static void chargerToutesLesDonnees() {
        chargerMatieres();
        chargerSalles();
        chargerCours();
        chargerPersonnes();
        chargerNotes();
        chargerPaiements();
    }

    static void sauvegarderToutesLesDonnees() {
        sauvegarderMatieres();
        sauvegarderSalles();
        sauvegarderCours();
        sauvegarderPersonnes();
        sauvegarderNotes();
        sauvegarderPaiements();
    }

    static void chargerMatieres() {
        ifstream f("matieres.txt");
        if (!f.is_open()) return;
        matieres.clear();
        string line;
        while (getline(f, line)) {
            if (line.empty()) continue;
            Matiere m;
            m.fromFileString(line);
            matieres.push_back(m);
            if (m.getId() >= nextMatiereId) nextMatiereId = m.getId() + 1;
        }
        f.close();
    }

    static void sauvegarderMatieres() {
        ofstream f("matieres.txt");
        for (const auto& m : matieres)
            f << m.toFileString() << "\n";
        f.close();
    }

    static void chargerSalles() {
        ifstream f("salles.txt");
        if (!f.is_open()) return;
        salles.clear();
        string line;
        while (getline(f, line)) {
            if (line.empty()) continue;
            Salle s;
            s.fromFileString(line);
            salles.push_back(s);
            if (s.getId() >= nextSalleId) nextSalleId = s.getId() + 1;
        }
        f.close();
    }

    static void sauvegarderSalles() {
        ofstream f("salles.txt");
        for (const auto& s : salles)
            f << s.toFileString() << "\n";
        f.close();
    }

    static void chargerCours() {
        ifstream f("cours.txt");
        if (!f.is_open()) return;
        cours.clear();
        string line;
        while (getline(f, line)) {
            if (line.empty()) continue;
            Cours c;
            c.fromFileString(line);
            cours.push_back(c);
            if (c.getId() >= nextCoursId) nextCoursId = c.getId() + 1;
        }
        f.close();
    }

    static void sauvegarderCours() {
        ofstream f("cours.txt");
        for (const auto& c : cours)
            f << c.toFileString() << "\n";
        f.close();
    }

    static void chargerPersonnes(); // Implementation after class definitions

    static void sauvegarderPersonnes() {
        ofstream f("personnes.txt");
        for (auto* p : personnes) {
            f << p->getType() << ";" << p->toFileString() << "\n";
        }
        f.close();
    }

    static void chargerNotes(); // Implementation after class definitions
    static void sauvegarderNotes(); // Implementation after class definitions
    static void chargerPaiements(); // Implementation after class definitions

    static void sauvegarderPaiements() {
        ofstream f("paiements.txt");
        for (const auto& p : paiements)
            f << p.toFileString() << "\n";
        f.close();
    }

    static void chargerRelationsEtudiants(); // Implementation after class definitions
    static Etudiant* trouverEtudiant(int id); // Implementation after class definitions

    static Personne* trouverPersonne(int id) {
        for (auto* p : personnes)
            if (p->getId() == id) return p;
        return nullptr;
    }

    static Personne* login(int id, string password){
        for (auto* p : personnes) {
            if (p->getId() == id) {
                if (p->getPassword() == password) {
                    return p;
                } else {
                    throw runtime_error("Mot de Pass est Incorrect");
                }
            }
        }
        throw runtime_error("Person non trouve");
    }

    static Matiere* trouverMatiere(int id) {
        for (auto& m : matieres)
            if (m.getId() == id) return &m;
        return nullptr;
    }

    static Salle* trouverSalle(int id) {
        for (auto& s : salles)
            if (s.getId() == id) return &s;
        return nullptr;
    }
};

// Static member definitions
vector<Personne*> DataManager::personnes;
vector<Matiere> DataManager::matieres;
vector<Salle> DataManager::salles;
vector<Cours> DataManager::cours;
vector<Etudiant*> DataManager::etudiants;
vector<Note> DataManager::notes;
vector<Paiement> DataManager::paiements;
int DataManager::nextPersonneId = 1;
int DataManager::nextMatiereId = 1;
int DataManager::nextSalleId = 1;
int DataManager::nextCoursId = 1;
int DataManager::nextNoteId = 1;
int DataManager::nextPaiementId = 1;

class Etudiant : public Personne {
private:
    vector<Matiere> matieres;
    vector<Note> notes;
    Emploi emploi;
    vector<Paiement> paiements;

public:
    Etudiant(int id, string prenom, string nom, string dateNaissance, string email)
        : Personne(id, prenom, nom, dateNaissance, email), emploi(id) {}

    string getType() const override { return "Etudiant"; }

    void consulterMatieres() const {
        cout << "\n--- MES MATIERES ---\n";
        if (matieres.empty())
            cout << "Aucune matiere.\n";
        else
            for (const auto& m : matieres)
                m.afficher();
    }

    void consulterNotes() const {
        cout << "\n--- MES NOTES ---\n";
        if (notes.empty())
            cout << "Aucune note.\n";
        else
            for (const auto& n : notes)
                n.afficher();
    }

    void consulterEmploi() const {
        emploi.consulter();
    }

    void consulterPaiements() const {
        cout << "\n--- MES PAIEMENTS ---\n";
        if (paiements.empty())
            cout << "Aucun paiement.\n";
        else
            for (const auto& p : paiements)
                p.afficher();
    }

    void ajouterPaiement(const Paiement& p) {
        paiements.push_back(p);
    }

    void ajouterCours(const Cours& c) {
        emploi.ajouterCours(c);
    }

    void ajouterNoteInterne(const Note& n) {
        notes.push_back(n);
    }

    void ajouterMatiere(const Matiere& m) {
        matieres.push_back(m);
    }

    friend class Enseignant;
    friend class Caissiere;
    friend class DirecteurPedagogique;
    friend class DataManager;

    void menu() override {
        int choix;
        do {
            cout << "\n===== MENU ETUDIANT =====\n";
            cout << "1. Afficher mes informations\n";
            cout << "2. Consulter mes matieres\n";
            cout << "3. Consulter mes notes\n";
            cout << "4. Consulter mon emploi du temps\n";
            cout << "5. Consulter mes paiements\n";
            cout << "6. Consulter mes absences / retards\n";
            cout << "7. Quitter\n";
            cout << "Choix : ";
            cin >> choix;
            cin.ignore();

            switch (choix) {
                case 1: afficherInfos(); break;
                case 2: consulterMatieres(); break;
                case 3: consulterNotes(); break;
                case 4: consulterEmploi(); break;
                case 5: consulterPaiements(); break;
                case 6:
                    cout << "Absences : " << getAbsence()
                         << " | Retards : " << getRetard() << endl;
                    break;
            }
        } while (choix != 7);
    }
};

class Bibliothecaire : public Personne {
public:
    Bibliothecaire(int id, string prenom, string nom, string dateNaissance, string email)
        : Personne(id, prenom, nom, dateNaissance, email) {}

    string getType() const override { return "Bibliothecaire"; }

    void enregistrerEmprunt(Etudiant& e, const string& livre) {
        ofstream f("bibliotheque.txt", ios::app);
        f << e.getId() << ";" << livre << ";" << "EMPRUNT\n";
        f.close();
        cout << "Emprunt enregistre.\n";
    }

    void enregistrerRetour(Etudiant& e, const string& livre) {
        ofstream f("bibliotheque.txt", ios::app);
        f << e.getId() << ";" << livre << ";" << "RETOUR\n";
        f.close();
        cout << "Retour enregistre.\n";
    }

    void consulterEmprunts() {
        ifstream f("bibliotheque.txt");
        if (!f.is_open()) {
            cout << "Aucun emprunt enregistre.\n";
            return;
        }
        cout << "\n--- LISTE DES EMPRUNTS ---\n";
        string line;
        while (getline(f, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            string id, livre, type;
            getline(ss, id, ';');
            getline(ss, livre, ';');
            getline(ss, type, ';');
            cout << "Etudiant " << id << " - " << livre << " (" << type << ")\n";
        }
        f.close();
    }

    void menu() override {
        int choix;
        do {
            cout << "\n===== MENU BIBLIOTHECAIRE =====\n";
            cout << "1. Enregistrer un emprunt\n";
            cout << "2. Enregistrer un retour\n";
            cout << "3. Consulter les emprunts\n";
            cout << "4. Quitter\n";
            cout << "Choix : ";
            cin >> choix;
            cin.ignore();

            if (choix == 1 || choix == 2) {
                int idE;
                string livre;
                cout << "ID Etudiant : ";
                cin >> idE;
                cin.ignore();
                cout << "Nom du livre : ";
                getline(cin, livre);
                Etudiant* e = DataManager::trouverEtudiant(idE);
                if (e) {
                    if (choix == 1) enregistrerEmprunt(*e, livre);
                    else enregistrerRetour(*e, livre);
                } else cout << "Etudiant non trouve.\n";
            } else if (choix == 3) {
                consulterEmprunts();
            }
        } while (choix != 4);
    }
};

class Caissiere : public Personne {
public:
    Caissiere(int id, string prenom, string nom, string dateNaissance, string email)
        : Personne(id, prenom, nom, dateNaissance, email) {}

    string getType() const override { return "Caissiere"; }

    void encaisserPaiement(Etudiant& e, float montant, const string& type = "Frais") {
        string date;
        cout << "Date (JJ/MM/AAAA) : ";
        cin >> date;
        Paiement p(DataManager::nextPaiementId++, e.getId(), montant, date, type);
        e.ajouterPaiement(p);
        p.sauvegarder();
        DataManager::paiements.push_back(p);
        cout << "Paiement encaisse avec succes.\n";
    }

    void menu() override {
        int choix;
        do {
            cout << "\n===== MENU CAISSIERE =====\n";
            cout << "1. Encaisser un paiement\n";
            cout << "2. Consulter les paiements\n";
            cout << "3. Quitter\n";
            cout << "Choix : ";
            cin >> choix;
            cin.ignore();

            if (choix == 1) {
                int idE;
                float montant;
                cout << "ID Etudiant : ";
                cin >> idE;
                cout << "Montant : ";
                cin >> montant;
                Etudiant* e = DataManager::trouverEtudiant(idE);
                if (e) {
                    encaisserPaiement(*e, montant);
                } else cout << "Etudiant non trouve.\n";
            } else if (choix == 2) {
                cout << "\n--- LISTE DES PAIEMENTS ---\n";
                for (const auto& p : DataManager::paiements)
                    p.afficher();
            }
        } while (choix != 3);
    }
};

class PersonnelMenage : public Personne {
    string zone;

public:
    PersonnelMenage(int id, string prenom, string nom, string dateNaissance, string email, string z)
        : Personne(id, prenom, nom, dateNaissance, email), zone(z) {}

    string getType() const override { return "PersonnelMenage"; }
    string getZone() const { return zone; }

    string toFileString() const override {
        return zone + ";" + Personne::toFileString();
    }

    void nettoyer() {
        cout << "Nettoyage de la zone : " << zone << endl;
        ofstream f("nettoyage.txt", ios::app);
        f << "Zone " << zone << " nettoyee le " << __DATE__ << "\n";
        f.close();
    }

    void menu() override {
        int choix;
        do {
            cout << "\n===== MENU PERSONNEL MENAGE =====\n";
            cout << "1. Nettoyer ma zone\n";
            cout << "2. Afficher mes informations\n";
            cout << "3. Quitter\n";
            cout << "Choix : ";
            cin >> choix;
            cin.ignore();

            if (choix == 1) nettoyer();
            else if (choix == 2) afficherInfos();
        } while (choix != 3);
    }
};

class DirecteurPedagogique : public Personne {
public:
    DirecteurPedagogique(int id, string prenom, string nom, string dateNaissance, string email)
        : Personne(id, prenom, nom, dateNaissance, email) {}

    string getType() const override { return "DirecteurPedagogique"; }

    void planifierCours(Etudiant& e, const Cours& c) {
        e.ajouterCours(c);
        c.sauvegarder();
        DataManager::cours.push_back(c);
        cout << "Cours planifie avec succes.\n";
    }

    void menu() override {
        int choix;
        do {
            cout << "\n===== MENU DIRECTEUR PEDAGOGIQUE =====\n";
            cout << "1. Planifier un cours\n";
            cout << "2. Consulter les cours\n";
            cout << "3. Consulter les matieres\n";
            cout << "4. Consulter les salles\n";
            cout << "5. Quitter\n";
            cout << "Choix : ";
            cin >> choix;
            cin.ignore();

            if (choix == 1) {
                int idE, idM, idS;
                string jour, hd, hf;
                cout << "ID Etudiant : ";
                cin >> idE;
                cout << "ID Matiere : ";
                cin >> idM;
                cout << "ID Salle : ";
                cin >> idS;
                cin.ignore();
                cout << "Jour : ";
                getline(cin, jour);
                cout << "Heure debut : ";
                getline(cin, hd);
                cout << "Heure fin : ";
                getline(cin, hf);
                Etudiant* e = DataManager::trouverEtudiant(idE);
                if (e) {
                    Cours c(DataManager::nextCoursId++, idM, idS, jour, hd, hf, idE);
                    planifierCours(*e, c);
                } else cout << "Etudiant non trouve.\n";
            } else if (choix == 2) {
                cout << "\n--- LISTE DES COURS ---\n";
                for (const auto& c : DataManager::cours)
                    c.afficher();
            } else if (choix == 3) {
                cout << "\n--- LISTE DES MATIERES ---\n";
                for (const auto& m : DataManager::matieres)
                    m.afficher();
            } else if (choix == 4) {
                cout << "\n--- LISTE DES SALLES ---\n";
                for (const auto& s : DataManager::salles)
                    s.afficher();
            }
        } while (choix != 5);
    }
};

class ResponsableSite : public Personne {
public:
    ResponsableSite(int id, string prenom, string nom, string dateNaissance, string email)
        : Personne(id, prenom, nom, dateNaissance, email) {}

    string getType() const override { return "ResponsableSite"; }

    void signalerProbleme(const string& description) {
        ofstream f("maintenance.txt", ios::app);
        f << "PROBLEME;" << description << ";" << __DATE__ << "\n";
        f.close();
        cout << "Probleme signale au service technique.\n";
    }

    void consulterProblemes() {
        ifstream f("maintenance.txt");
        if (!f.is_open()) {
            cout << "Aucun probleme signale.\n";
            return;
        }
        cout << "\n--- LISTE DES PROBLEMES ---\n";
        string line;
        while (getline(f, line)) {
            if (line.empty()) continue;
            cout << line << endl;
        }
        f.close();
    }

    void menu() override {
        int choix;
        do {
            cout << "\n===== MENU RESPONSABLE SITE =====\n";
            cout << "1. Signaler un probleme\n";
            cout << "2. Consulter les problemes\n";
            cout << "3. Quitter\n";
            cout << "Choix : ";
            cin >> choix;
            cin.ignore();

            if (choix == 1) {
                string desc;
                cout << "Description du probleme : ";
                getline(cin, desc);
                signalerProbleme(desc);
            } else if (choix == 2) {
                consulterProblemes();
            }
        } while (choix != 3);
    }
};

class Technicien : public Personne {
public:
    Technicien(int id, string prenom, string nom, string dateNaissance, string email)
        : Personne(id, prenom, nom, dateNaissance, email) {}

    string getType() const override { return "Technicien"; }

    void reparer(const string& intervention) {
        ofstream f("maintenance.txt", ios::app);
        f << "REPARATION;" << intervention << ";" << __DATE__ << "\n";
        f.close();
        cout << "Intervention technique enregistree.\n";
    }

    void consulterInterventions() {
        ifstream f("maintenance.txt");
        if (!f.is_open()) {
            cout << "Aucune intervention.\n";
            return;
        }
        cout << "\n--- LISTE DES INTERVENTIONS ---\n";
        string line;
        while (getline(f, line)) {
            if (line.empty()) continue;
            cout << line << endl;
        }
        f.close();
    }

    void menu() override {
        int choix;
        do {
            cout << "\n===== MENU TECHNICIEN =====\n";
            cout << "1. Enregistrer une reparation\n";
            cout << "2. Consulter les interventions\n";
            cout << "3. Quitter\n";
            cout << "Choix : ";
            cin >> choix;
            cin.ignore();

            if (choix == 1) {
                string inter;
                cout << "Description de l'intervention : ";
                getline(cin, inter);
                reparer(inter);
            } else if (choix == 2) {
                consulterInterventions();
            }
        } while (choix != 3);
    }
};

class SurveillantGeneral : public Personne {
public:
    SurveillantGeneral(int id, string prenom, string nom, string dateNaissance, string email)
        : Personne(id, prenom, nom, dateNaissance, email) {}

    string getType() const override { return "SurveillantGeneral"; }

    void enregistrerAbsence(Personne& p) {
        p.ajouterAbsence();
        enregistrerAction("Absence", p.getId());
        DataManager::sauvegarderPersonnes();
        cout << "Absence enregistree pour " << p.getNomComplet() << endl;
    }

    void enregistrerRetard(Personne& p) {
        p.ajouterRetard();
        enregistrerAction("Retard", p.getId());
        DataManager::sauvegarderPersonnes();
        cout << "Retard enregistre pour " << p.getNomComplet() << endl;
    }

    void consulterabsence() {
        ifstream f("absence.txt");
        if (!f.is_open()) {
            cout << "Aucun enregistrement.\n";
            return;
        }
        cout << "\n--- ENREGISTREMENTS absence ---\n";
        string line;
        while (getline(f, line)) {
            if (line.empty()) continue;
            cout << line << endl;
        }
        f.close();
    }

private:
    void enregistrerAction(const string& type, int idPersonne) {
        ofstream f("absence.txt", ios::app);
        f << type << ";" << idPersonne << ";" << __DATE__ << "\n";
        f.close();
    }

public:
    void menu() override {
        int choix;
        do {
            cout << "\n===== MENU SURVEILLANT GENERAL =====\n";
            cout << "1. Enregistrer une absence\n";
            cout << "2. Enregistrer un retard\n";
            cout << "3. Consulter les enregistrements\n";
            cout << "4. Quitter\n";
            cout << "Choix : ";
            cin >> choix;
            cin.ignore();

            if (choix == 1 || choix == 2) {
                int idP;
                cout << "ID Personne : ";
                cin >> idP;
                Personne* p = DataManager::trouverPersonne(idP);
                if (p) {
                    if (choix == 1) enregistrerAbsence(*p);
                    else enregistrerRetard(*p);
                } else cout << "Personne non trouvee.\n";
            } else if (choix == 3) {
                consulterabsence();
            }
        } while (choix != 4);
    }
};

class Enseignant : public Personne {
public:
    Enseignant(int id, string prenom, string nom, string dateNaissance, string email)
        : Personne(id, prenom, nom, dateNaissance, email) {}

    string getType() const override { return "Enseignant"; }

    void ajouterNote(Etudiant& e, const Note& n) {
        e.notes.push_back(n);
        n.sauvegarder(e.getId());
        DataManager::notes.push_back(n);
        cout << "Note ajoutee avec succes.\n";
    }

    void modifierNote(Etudiant& e, int index, float nouvelleValeur) {
        if (index >= 0 && index < (int)e.notes.size()) {
            e.notes[index].modifier(nouvelleValeur);
            cout << "Note modifiee.\n";
            DataManager::sauvegarderNotes();
        } else {
            cout << "Index invalide.\n";
        }
    }

    void supprimerNote(Etudiant& e, int index) {
        if (index >= 0 && index < (int)e.notes.size()) {
            e.notes.erase(e.notes.begin() + index);
            cout << "Note supprimee.\n";
            DataManager::sauvegarderNotes();
        } else {
            cout << "Index invalide.\n";
        }
    }

    void menu() override {
        int choix;
        do {
            cout << "\n===== MENU ENSEIGNANT =====\n";
            cout << "1. Ajouter une note\n";
            cout << "2. Modifier une note\n";
            cout << "3. Supprimer une note\n";
            cout << "4. Consulter les etudiants\n";
            cout << "5. Consulter les matieres\n";
            cout << "6. Quitter\n";
            cout << "Choix : ";
            cin >> choix;
            cin.ignore();

            if (choix == 1) {
                int idE, idM;
                float val;
                string date;
                cout << "ID Etudiant : ";
                cin >> idE;
                cout << "ID Matiere : ";
                cin >> idM;
                cout << "Valeur : ";
                cin >> val;
                cin.ignore();
                cout << "Date (JJ/MM/AAAA) : ";
                getline(cin, date);
                Etudiant* e = DataManager::trouverEtudiant(idE);
                if (e) {
                    Note n(DataManager::nextNoteId++, val, date, idM);
                    ajouterNote(*e, n);
                } else cout << "Etudiant non trouve.\n";
            } else if (choix == 2) {
                int idE, index;
                float val;
                cout << "ID Etudiant : ";
                cin >> idE;
                cout << "Index de la note : ";
                cin >> index;
                cout << "Nouvelle valeur : ";
                cin >> val;
                Etudiant* e = DataManager::trouverEtudiant(idE);
                if (e) modifierNote(*e, index, val);
                else cout << "Etudiant non trouve.\n";
            } else if (choix == 3) {
                int idE, index;
                cout << "ID Etudiant : ";
                cin >> idE;
                cout << "Index de la note : ";
                cin >> index;
                Etudiant* e = DataManager::trouverEtudiant(idE);
                if (e) supprimerNote(*e, index);
                else cout << "Etudiant non trouve.\n";
            } else if (choix == 4) {
                cout << "\n--- LISTE DES ETUDIANTS ---\n";
                for (auto* e : DataManager::etudiants)
                    e->afficherInfos();
            } else if (choix == 5) {
                cout << "\n--- LISTE DES MATIERES ---\n";
                for (const auto& m : DataManager::matieres)
                    m.afficher();
            }
        } while (choix != 6);
    }
};

class Infirmier : public Personne {
public:
    Infirmier(int id, string prenom, string nom, string dateNaissance, string email)
        : Personne(id, prenom, nom, dateNaissance, email) {}

    string getType() const override { return "Infirmier"; }

    void enregistrerSoin(Personne& p, const string& soin) {
        ofstream f("infirmerie.txt", ios::app);
        f << p.getId() << ";" << p.getNomComplet() << ";" << soin << ";" << __DATE__ << "\n";
        f.close();
        cout << "Soin enregistre pour " << p.getNomComplet() << endl;
    }

    void consulterSoins() {
        ifstream f("infirmerie.txt");
        if (!f.is_open()) {
            cout << "Aucun soin enregistre.\n";
            return;
        }
        cout << "\n--- LISTE DES SOINS ---\n";
        string line;
        while (getline(f, line)) {
            if (line.empty()) continue;
            cout << line << endl;
        }
        f.close();
    }

    void menu() override {
        int choix;
        do {
            cout << "\n===== MENU INFIRMIER =====\n";
            cout << "1. Enregistrer un soin\n";
            cout << "2. Consulter les soins\n";
            cout << "3. Quitter\n";
            cout << "Choix : ";
            cin >> choix;
            cin.ignore();

            if (choix == 1) {
                int idP;
                string soin;
                cout << "ID Personne : ";
                cin >> idP;
                cin.ignore();
                cout << "Description du soin : ";
                getline(cin, soin);
                Personne* p = DataManager::trouverPersonne(idP);
                if (p) enregistrerSoin(*p, soin);
                else cout << "Personne non trouvee.\n";
            } else if (choix == 2) {
                consulterSoins();
            }
        } while (choix != 3);
    }
};

class AgentSecurite : public Personne {
public:
    AgentSecurite(int id, string prenom, string nom, string dateNaissance, string email)
        : Personne(id, prenom, nom, dateNaissance, email) {}

    string getType() const override { return "AgentSecurite"; }

    void signalerIncident(const string& typeIncident, const string& lieu, const string& description) {
        ofstream f("securite.txt", ios::app);
        f << typeIncident << ";" << lieu << ";" << description << ";" << __DATE__ << "\n";
        f.close();
        cout << "Incident de securite enregistre.\n";
    }

    void consulterIncidents() {
        ifstream f("securite.txt");
        if (!f.is_open()) {
            cout << "Aucun incident enregistre.\n";
            return;
        }
        cout << "\n--- LISTE DES INCIDENTS ---\n";
        string line;
        while (getline(f, line)) {
            if (line.empty()) continue;
            cout << line << endl;
        }
        f.close();
    }

    void menu() override {
        int choix;
        do {
            cout << "\n===== MENU AGENT DE SECURITE =====\n";
            cout << "1. Signaler un incident\n";
            cout << "2. Consulter les incidents\n";
            cout << "3. Quitter\n";
            cout << "Choix : ";
            cin >> choix;
            cin.ignore();

            if (choix == 1) {
                string type, lieu, desc;
                cout << "Type d'incident : ";
                getline(cin, type);
                cout << "Lieu : ";
                getline(cin, lieu);
                cout << "Description : ";
                getline(cin, desc);
                signalerIncident(type, lieu, desc);
            } else if (choix == 2) {
                consulterIncidents();
            }
        } while (choix != 3);
    }
};

// Implementations of DataManager methods (after all class definitions)
void DataManager::chargerNotes() {
    ifstream f("notes.txt");
    if (!f.is_open()) return;
    notes.clear();
    string line;
    while (getline(f, line)) {
        if (line.empty()) continue;
        Note n;
        int idE;
        n.fromFileString(line, idE);
        notes.push_back(n);
        Etudiant* e = trouverEtudiant(idE);
        if (e) e->ajouterNoteInterne(n);
        if (n.getId() >= nextNoteId) nextNoteId = n.getId() + 1;
    }
    f.close();
}

void DataManager::sauvegarderNotes() {
    ofstream f("notes.txt");
    // Save notes from all students (DataManager is friend of Etudiant)
    for (auto* e : etudiants) {
        for (const auto& n : e->notes) {
            f << n.toFileString(e->getId()) << "\n";
        }
    }
    f.close();
}

void DataManager::chargerPaiements() {
    ifstream f("paiements.txt");
    if (!f.is_open()) return;
    paiements.clear();
    string line;
    while (getline(f, line)) {
        if (line.empty()) continue;
        Paiement p;
        p.fromFileString(line);
        paiements.push_back(p);
        Etudiant* e = trouverEtudiant(p.getIdEtudiant());
        if (e) e->ajouterPaiement(p);
        if (p.getId() >= nextPaiementId) nextPaiementId = p.getId() + 1;
    }
    f.close();
}

void DataManager::chargerRelationsEtudiants() {
    // Load student relations (matieres, notes, cours, paiements)
    for (auto* e : etudiants) {
        // Load matieres for student (simplified - would need separate file)
        // Load cours for student
        for (const auto& c : cours) {
            if (c.getIdEtudiant() == e->getId()) {
                e->ajouterCours(c);
            }
        }
    }
}

Etudiant* DataManager::trouverEtudiant(int id) {
    for (auto* e : etudiants)
        if (e->getId() == id) return e;
    return nullptr;
}

void DataManager::chargerPersonnes() {
    ifstream f("personnes.txt");
    if (!f.is_open()) return;
    personnes.clear();
    etudiants.clear();
    string line;
    while (getline(f, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string type;
        getline(ss, type, ';');
        string data = line.substr(type.length() + 1);
        
        Personne* p = nullptr;
        if (type == "Etudiant") {
            Etudiant* e = new Etudiant(0, "", "", "", "");
            e->fromFileString(data);
            p = e;
            etudiants.push_back(e);
        } else if (type == "Enseignant") {
            p = new Enseignant(0, "", "", "", "");
            p->fromFileString(data);
        } else if (type == "Caissiere") {
            p = new Caissiere(0, "", "", "", "");
            p->fromFileString(data);
        } else if (type == "DirecteurPedagogique") {
            p = new DirecteurPedagogique(0, "", "", "", "");
            p->fromFileString(data);
        } else if (type == "Bibliothecaire") {
            p = new Bibliothecaire(0, "", "", "", "");
            p->fromFileString(data);
        } else if (type == "SurveillantGeneral") {
            p = new SurveillantGeneral(0, "", "", "", "");
            p->fromFileString(data);
        } else if (type == "Infirmier") {
            p = new Infirmier(0, "", "", "", "");
            p->fromFileString(data);
        } else if (type == "AgentSecurite") {
            p = new AgentSecurite(0, "", "", "", "");
            p->fromFileString(data);
        } else if (type == "Technicien") {
            p = new Technicien(0, "", "", "", "");
            p->fromFileString(data);
        } else if (type == "ResponsableSite") {
            p = new ResponsableSite(0, "", "", "", "");
            p->fromFileString(data);
        } else if (type == "PersonnelMenage") {
            stringstream ss2(data);
            string zone;
            getline(ss2, zone, ';');
            string item;
            getline(ss2, item, ';'); int id = stoi(item);
            string prenom, nom, date, email, tel, adr;
            getline(ss2, prenom, ';');
            getline(ss2, nom, ';');
            getline(ss2, date, ';');
            getline(ss2, email, ';');
            getline(ss2, tel, ';');
            getline(ss2, adr, ';');
            getline(ss2, item, ';'); bool ac = (item == "1");
            getline(ss2, item, ';'); int a = stoi(item);
            getline(ss2, item, ';'); int r = stoi(item);
            PersonnelMenage* pm = new PersonnelMenage(id, prenom, nom, date, email, zone);
            pm->setTelephone(tel);
            pm->setAdresse(adr);
            pm->setActif(ac);
            pm->setAbsence(a);
            pm->setRetard(r);
            p = pm;
        }
        if (p) {
            personnes.push_back(p);
            if (p->getId() >= nextPersonneId) nextPersonneId = p->getId() + 1;
        }
    }
    f.close();
    chargerRelationsEtudiants();
}

// Main function with authentication and menu system
int main() {
    cout << "===== SYSTEME DE GESTION SCOLAIRE =====\n\n";
    
    // Initialize database files if they don't exist (create empty files)
    vector<string> dbFiles = {
        "personnes.txt", "matieres.txt", "notes.txt", "salles.txt",
        "cours.txt", "paiements.txt", "bibliotheque.txt", "absence.txt",
        "maintenance.txt", "infirmerie.txt", "securite.txt", "nettoyage.txt"
    };
    for (const auto& filename : dbFiles) {
        ifstream check(filename);
        if (!check.is_open()) {
            // File doesn't exist, create empty file
            ofstream create(filename);
            create.close();
        } else {
            check.close();
        }
    }
    
    // Load all data
    DataManager::chargerToutesLesDonnees();
    
    // Main menu
    int choix;
    Personne* currentUser = nullptr;
    
    do {
        cout << "\n===== MENU PRINCIPAL =====\n";
        cout << "1. Login\n";
        // cout << "2. Creer un compte\n";
        // cout << "3. Gestion des matieres\n";
        // cout << "4. Gestion des salles\n";
        cout << "5. Quitter\n";
        cout << "Choix : ";
        cin >> choix;
        cin.ignore();

        if (choix == 1) {
            int id;
            string password;
            cout << "Entrez votre ID : ";
            cin >> id;
            cin.ignore(); // Clear the input buffer
            cout << "Entrez votre Mot de Passe : ";
            getline(cin, password); // Use getline to handle passwords properly
            currentUser = nullptr; // Reset currentUser before login attempt
            try {
                currentUser = DataManager::login(id, password);
            } catch (const runtime_error& e) {
                cout << e.what() << endl;
                currentUser = nullptr; // Ensure currentUser is null on error
            }

            if (currentUser) {
                cout << "Bienvenue " << currentUser->getNomComplet() << "!\n";
                currentUser->menu();
            }
            //  else {
            //     cout << "Person non trouvee.\n";
            // }
        } else if (choix == 2) {
            int type;
            cout << "Type de compte:\n";
            cout << "1. Etudiant\n2. Enseignant\n3. Caissiere\n4. Directeur Pedagogique\n";
            cout << "5. Bibliothecaire\n6. Surveillant General\n7. Infirmier\n";
            cout << "8. Agent Securite\n9. Technicien\n10. Responsable Site\n11. Personnel Menage\n";
            cout << "Choix : ";
            cin >> type;
            cin.ignore();
            
            int id = DataManager::nextPersonneId++;
            string prenom, nom, date, email;
            cout << "Prenom : ";
            getline(cin, prenom);
            cout << "Nom : ";
            getline(cin, nom);
            cout << "Date de naissance (JJ/MM/AAAA) : ";
            getline(cin, date);
            cout << "Email : ";
            getline(cin, email);

            Personne* p = nullptr;
            if (type == 1) {
                p = new Etudiant(id, prenom, nom, date, email);
                DataManager::etudiants.push_back((Etudiant*)p);
            } else if (type == 2) {
                p = new Enseignant(id, prenom, nom, date, email);
            } else if (type == 3) {
                p = new Caissiere(id, prenom, nom, date, email);
            } else if (type == 4) {
                p = new DirecteurPedagogique(id, prenom, nom, date, email);
            } else if (type == 5) {
                p = new Bibliothecaire(id, prenom, nom, date, email);
            } else if (type == 6) {
                p = new SurveillantGeneral(id, prenom, nom, date, email);
            } else if (type == 7) {
                p = new Infirmier(id, prenom, nom, date, email);
            } else if (type == 8) {
                p = new AgentSecurite(id, prenom, nom, date, email);
            } else if (type == 9) {
                p = new Technicien(id, prenom, nom, date, email);
            } else if (type == 10) {
                p = new ResponsableSite(id, prenom, nom, date, email);
            } else if (type == 11) {
                string zone;
                cout << "Zone : ";
                getline(cin, zone);
                p = new PersonnelMenage(id, prenom, nom, date, email, zone);
            }
            
            if (p) {
                DataManager::personnes.push_back(p);
                DataManager::sauvegarderPersonnes();
                cout << "Compte cree avec succes. Votre ID est : " << id << endl;
            }
        } else if (choix == 3) {
            // Gestion des matieres
            int choixM;
            cout << "\n===== GESTION DES MATIERES =====\n";
            cout << "1. Ajouter une matiere\n";
            cout << "2. Consulter les matieres\n";
            cout << "3. Modifier une matiere\n";
            cout << "Choix : ";
            cin >> choixM;
            cin.ignore();
            
            if (choixM == 1) {
                string nom, desc;
                int coef;
                cout << "Nom : ";
                getline(cin, nom);
                cout << "Coefficient : ";
                cin >> coef;
                cin.ignore();
                cout << "Description : ";
                getline(cin, desc);
                Matiere m(DataManager::nextMatiereId++, nom, coef, desc);
                m.sauvegarder();
                DataManager::matieres.push_back(m);
                cout << "Matiere ajoutee.\n";
            } else if (choixM == 2) {
                for (const auto& m : DataManager::matieres)
                    m.afficher();
            } else if (choixM == 3) {
                int id;
                cout << "ID Matiere : ";
                cin >> id;
                Matiere* m = DataManager::trouverMatiere(id);
                if (m) {
                    string nom, desc;
                    int coef;
                    cout << "Nouveau nom : ";
                    cin.ignore();
                    getline(cin, nom);
                    cout << "Nouveau coefficient : ";
                    cin >> coef;
                    cin.ignore();
                    cout << "Nouvelle description : ";
                    getline(cin, desc);
                    m->modifier(nom, coef, desc);
                    DataManager::sauvegarderMatieres();
                    cout << "Matiere modifiee.\n";
                }
            }
        } else if (choix == 4) {
            // Gestion des salles
            int choixS;
            cout << "\n===== GESTION DES SALLES =====\n";
            cout << "1. Ajouter une salle\n";
            cout << "2. Consulter les salles\n";
            cout << "3. Reserver/Liberer une salle\n";
            cout << "Choix : ";
            cin >> choixS;
            cin.ignore();
            
            if (choixS == 1) {
                string num;
                int cap, etage;
                cout << "Numero : ";
                getline(cin, num);
                cout << "Capacite : ";
                cin >> cap;
                cout << "Etage : ";
                cin >> etage;
                Salle s(DataManager::nextSalleId++, num, cap, etage);
                s.sauvegarder();
                DataManager::salles.push_back(s);
                cout << "Salle ajoutee.\n";
            } else if (choixS == 2) {
                for (const auto& s : DataManager::salles)
                    s.afficher();
            } else if (choixS == 3) {
                int id;
                cout << "ID Salle : ";
                cin >> id;
                Salle* s = DataManager::trouverSalle(id);
                if (s) {
                    int action;
                    cout << "1. Reserver\n2. Liberer\n";
                    cin >> action;
                    if (action == 1) s->reserver();
                    else s->liberer();
                    DataManager::sauvegarderSalles();
                }
            }
        }
    } while (choix != 5);

    // Save all data before exit
    DataManager::sauvegarderToutesLesDonnees();
    cout << "Donnees sauvegardees. Au revoir!\n";
    
    // Cleanup
    for (auto* p : DataManager::personnes)
        delete p;
    
    return 0;
}


//compile in vs code by ameur
// g++ -std=c++11 final.cpp -o final