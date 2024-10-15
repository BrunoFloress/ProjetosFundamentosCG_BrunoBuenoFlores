#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <vector>

// Configurações da janela e jogo
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const float GRAVITY = 800.0f;
const float JUMP_SPEED = 600.0f;
const float MOVE_SPEED = 250.0f;

// Estrutura do Jogador (Cachorro)
struct Player {
    sf::RectangleShape shape;
    sf::Vector2f velocity;
    bool isJumping;
    sf::Texture texture; // Textura do cachorro

    // Carregar textura
    void loadTexture(const std::string& filename) {
        if (!texture.loadFromFile(filename)) {
            std::cerr << "Erro ao carregar a textura do cachorro!" << std::endl;
        }
        shape.setTexture(&texture); // Aplica a textura
    }
};

// Estrutura dos Obstáculos (Latas de Lixo)
struct Obstacle {
    sf::RectangleShape shape;
    sf::Texture texture; // Textura da lata de lixo

    // Carregar textura
    void loadTexture(const std::string& filename) {
        if (!texture.loadFromFile(filename)) {
            std::cerr << "Erro ao carregar a textura da lata de lixo!" << std::endl;
        }
        shape.setTexture(&texture); // Aplica a textura
    }
};

// Estrutura dos Ossos (Itens colecionáveis)
struct Bone {
    sf::RectangleShape shape;
    sf::Texture texture; // Textura do osso

    // Carregar textura
    void loadTexture(const std::string& filename) {
        if (!texture.loadFromFile(filename)) {
            std::cerr << "Erro ao carregar a textura do osso!" << std::endl;
        }
        shape.setTexture(&texture); // Aplica a textura
    }
};

void handleInput(Player& player) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !player.isJumping) {
        player.velocity.y = -JUMP_SPEED; // Faz o cachorro pular
        player.isJumping = true;
    }
}

void applyGravity(Player& player, float deltaTime) {
    player.velocity.y += GRAVITY * deltaTime;
    player.shape.move(0.0f, player.velocity.y * deltaTime);
}

// Verifica colisão do jogador com o chão e obstáculos
void checkCollision(Player& player, const sf::RectangleShape& ground) {
    if (player.shape.getGlobalBounds().intersects(ground.getGlobalBounds())) {
        player.velocity.y = 0.0f;
        player.isJumping = false;
        player.shape.setPosition(player.shape.getPosition().x, ground.getPosition().y - player.shape.getSize().y);
    }
}

void resetObstacle(Obstacle& obstacle) {
    obstacle.shape.setPosition(WINDOW_WIDTH + rand() % 200, WINDOW_HEIGHT - 100);
}

void resetBone(Bone& bone) {
    bone.shape.setPosition(WINDOW_WIDTH + rand() % 500, WINDOW_HEIGHT - 300);
}

int main() {
    // Criar a janela
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Endless Runner - Cachorro");
    window.setFramerateLimit(60);

    // Configurações do jogador (Cachorro)
    Player player;
    player.shape.setSize({ 50.0f, 50.0f });
    player.shape.setPosition(100.0f, WINDOW_HEIGHT - 100);
    player.loadTexture("assets/dog.png"); // Carregar textura do cachorro
    player.isJumping = false;
    player.velocity = { 0.0f, 0.0f };

    // Configurações do chão
    sf::RectangleShape ground;
    ground.setSize({ WINDOW_WIDTH, 50.0f });
    ground.setPosition(0.0f, WINDOW_HEIGHT - 50);
    ground.setFillColor(sf::Color::Green); // Cor do chão

    // Configurações do obstáculo (Latas de Lixo)
    std::vector<Obstacle> obstacles;
    for (int i = 0; i < 3; ++i) {
        Obstacle obstacle;
        obstacle.shape.setSize({ 30.0f, 70.0f });
        obstacle.loadTexture("assets/trash_can.png"); // Carregar textura da lata de lixo
        resetObstacle(obstacle);
        obstacles.push_back(obstacle);
    }

    // Configurações dos ossos (Itens colecionáveis)
    std::vector<Bone> bones;
    for (int i = 0; i < 2; ++i) {
        Bone bone;
        bone.shape.setSize({ 20.0f, 20.0f });
        bone.loadTexture("assets/bone.png"); // Carregar textura do osso
        resetBone(bone);
        bones.push_back(bone);
    }

    // Variável de pontuação
    int score = 0;
    sf::Font font;
    // Carregue a fonte se necessário aqui

    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);

    sf::Clock clock;
    while (window.isOpen()) {
        // Processar eventos
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        float deltaTime = clock.restart().asSeconds();

        // Lidar com entrada do jogador (salto)
        handleInput(player);

        // Aplicar gravidade e checar colisão com o chão
        applyGravity(player, deltaTime);
        checkCollision(player, ground);

        // Mover os obstáculos e ossos
        for (auto& obstacle : obstacles) {
            obstacle.shape.move(-MOVE_SPEED * deltaTime, 0.0f);
            if (obstacle.shape.getPosition().x < -obstacle.shape.getSize().x) {
                resetObstacle(obstacle);
            }
            if (player.shape.getGlobalBounds().intersects(obstacle.shape.getGlobalBounds())) {
                window.close(); // Colidiu com o obstáculo, fim de jogo
            }
        }

        for (auto& bone : bones) {
            bone.shape.move(-MOVE_SPEED * deltaTime, 0.0f);
            if (bone.shape.getPosition().x < -bone.shape.getSize().x) {
                resetBone(bone);
            }
            if (player.shape.getGlobalBounds().intersects(bone.shape.getGlobalBounds())) {
                score++;
                resetBone(bone); // Pegou o osso, reinicia sua posição
            }
        }

        // Atualizar o texto de pontuação
        scoreText.setString("Pontuacao: " + std::to_string(score));

        // Desenhar todos os elementos
        window.clear();
        window.draw(ground);
        window.draw(player.shape);
        for (const auto& obstacle : obstacles) {
            window.draw(obstacle.shape);
        }
        for (const auto& bone : bones) {
            window.draw(bone.shape);
        }
        window.draw(scoreText);
        window.display();
    }

    return 0;
}
