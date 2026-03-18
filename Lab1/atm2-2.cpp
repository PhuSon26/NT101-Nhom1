#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <random>
#include <regex>
#include <cmath>

class SubstitutionCipherSolver {
private:
    std::unordered_map<std::string, double> quadgrams;
    std::mt19937 rng;

public:
    SubstitutionCipherSolver() {
      
        std::random_device rd;
        rng.seed(rd());
    }

   
    bool LoadQuadgrams(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Loi: Khong the mo file '" << filename << "'. Vui long kiem tra lai duong dan!\n";
            return false;
        }

        std::string line;
        double total = 0;
        std::vector<std::pair<std::string, double>> temp_quads;

       
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string quad;
            double count;
            if (iss >> quad >> count) {
                temp_quads.push_back({quad, count});
                total += count;
            }
        }

        file.close();

        
        quadgrams.clear();
        for (const auto& pair : temp_quads) {
            quadgrams[pair.first] = std::log10(pair.second / total);
        }
        
        std::cout << "[+] Da tai thanh cong " << quadgrams.size() << " quadgrams tu file.\n";
        return true;
    }

    
    std::string Normalize(const std::string& text) {
        std::string result = text;
        std::transform(result.begin(), result.end(), result.begin(), ::toupper);
        result = std::regex_replace(result, std::regex("[^A-Z]"), "");
        return result;
    }

   
    double CalculateFitness(const std::string& text) {
        double score = 0;
        if (text.length() < 4) return score;

        for (size_t i = 0; i <= text.length() - 4; ++i) {
            std::string quad = text.substr(i, 4);
            auto it = quadgrams.find(quad);
            if (it != quadgrams.end()) {
                score += it->second;
            } else {
               
                score += -10.0; 
            }
        }
        return score;
    }

   
    std::string Decrypt(const std::string& cipher, const std::string& key) {
        std::string plaintext = "";
        plaintext.reserve(cipher.length());
        
        for (char c : cipher) {
            size_t index = key.find(c);
            if (index != std::string::npos) {
                plaintext += (char)('A' + index);
            } else {
                plaintext += c; 
            }
        }
        return plaintext;
    }

  
    std::string BreakCipher(std::string ciphertext, int restarts = 20) {
        std::string normalizedCipher = Normalize(ciphertext);
        
        std::string bestOverallKey = "";
        double bestOverallFitness = -1e9; 
        std::string bestPlaintext = "";

        std::uniform_int_distribution<int> dist(0, 25);

        for (int restart = 0; restart < restarts; ++restart) {
           
            std::string currentKey = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
            std::shuffle(currentKey.begin(), currentKey.end(), rng);
            
            double currentFitness = CalculateFitness(Decrypt(normalizedCipher, currentKey));
            int countWithoutImprovement = 0;

            
            while (countWithoutImprovement < 1000) {
                std::string childKey = currentKey;
              
                int idx1 = dist(rng);
                int idx2 = dist(rng);
                std::swap(childKey[idx1], childKey[idx2]);

                double childFitness = CalculateFitness(Decrypt(normalizedCipher, childKey));

               
                if (childFitness > currentFitness) {
                    currentKey = childKey;
                    currentFitness = childFitness;
                    countWithoutImprovement = 0;
                } else {
                    countWithoutImprovement++;
                }
            }

           
            if (currentFitness > bestOverallFitness) {
                bestOverallFitness = currentFitness;
                bestOverallKey = currentKey;
                
            
                bestPlaintext = "";
                for(char c : ciphertext) {
                    if(isalpha(c)) {
                        bool isLower = islower(c);
                        char upperC = toupper(c);
                        size_t idx = bestOverallKey.find(upperC);
                        char plainChar = (char)('A' + idx);
                        bestPlaintext += isLower ? tolower(plainChar) : plainChar;
                    } else {
                        bestPlaintext += c;
                    }
                }
            }
        }

        std::cout << "[+] Khoa tot nhat tim duoc: " << bestOverallKey << "\n";
       
        return bestPlaintext;
    }
};

int main() {
    SubstitutionCipherSolver solver;
    
   
    std::string dictionaryFile = "english-quadgrams.txt";
    if (!solver.LoadQuadgrams(dictionaryFile)) {
        return 1; 
    }
    
   
   std::string cipher;
   getline(std::cin,cipher);
    std::string plaintext = solver.BreakCipher(cipher, 20); 
    
    std::cout << "--- KET QUA GIAI MA ---\n";
    std::cout << plaintext << "\n";

    return 0;
}