# Space Shooter 

Sebuah game horizontal shooter yang dikembangkan dengan **C++17** dan **SFML 3**, menampilkan gameplay yang adiktif, sistem difficulty yang dinamis, dan sistem unlock bullet color berdasarkan achievement.

**Developed by:** [Your Name]  
**Status:** ✅ Complete & Playable

---

##  Daftar Isi

- [Fitur Utama](#fitur-utama)
- [Gameplay](#gameplay)
- [Sistem Difficulty (Cortisol)](#sistem-difficulty-cortisol)
- [Sistem Bullet Color](#sistem-bullet-color)
- [Kontrol Game](#kontrol-game)
- [Setup & Build](#setup--build)
- [Struktur Project](#struktur-project)
- [Technical Highlights](#technical-highlights)
- [Asset & Dependency](#asset--dependency)

---

##  Fitur Utama

### 1. **Horizontal Shooter Gameplay**
- Player mengendalikan pesawat di sisi kiri layar
- Obstacle (asteroid) muncul dari kanan dan bergerak ke kiri
- Pemain harus menembak obstacle untuk mendapatkan score
- Obstacle yang lolos akan mengurangi health

### 2. **Sistem Health & Life System**
- Player memiliki **3 health** pada awal permainan
- Setiap obstacle yang lolos mengurangi health 1
- Game over ketika health mencapai 0
- Tabrakan langsung dengan obstacle juga trigger game over
- **Damage Flash** visual feedback (layar berkilau merah) saat terkena damage

### 3. **Dynamic Difficulty System (Cortisol)**
Tingkat kesulitan dapat dipilih di menu utama:

| Level | Obstacle Speed | Spawn Rate | Deskripsi |
|-------|--------|-----------|-----------|
| **Low** | 3.0 px/frame | Setiap 1.8 detik | Santai, cocok untuk pemula |
| **Medium** | 5.5 px/frame | Setiap 1.0 detik | Balanced, challenge moderat |
| **High** | 9.0 px/frame | Setiap 0.45 detik | Intense, untuk pro players |

### 4. **Bullet Color System dengan Achievement**
Unlock warna peluru baru dengan mencapai score milestone:

| Warna | Required Score | Kondisi | Efek Khusus |
|-------|--------|---------|------------|
| **Basic** (Kuning) | 0 | Available from start | Standar, normal speed |
| **Blue** (Biru) | 1,000 | Play on Low difficulty | +35% faster, triangle design |
| **Red** (Merah) | 5,000 | Play on Medium difficulty | Normal speed, unique shape |
| **Purple** (Ungu) | 10,000 | Play on High difficulty | **Pierce 1 obstacle**, special glow effect |

**Purple Bullet Highlight:**
- Hanya warna Purple yang bisa menembus obstacle (pierce mechanic)
- Sangat berguna saat obstacle spawn rate tinggi
- Design visual yang menonjol dengan glow effect

### 5. **Collision Detection**
- **Bounding Box Collision** untuk performa optimal
- **Pixel-Perfect Collision** untuk akurasi tinggi (checks alpha > 30)
- Efisien: hanya melakukan pixel-check setelah bounding box collision terjadi
- Mencegah "unfair" collision dengan transparent pixel

### 6. **Particle-Based Explosion System**
Efek ledakan visual yang memukau saat obstacle terkena:

- **26 flame particles**: Api kuning/orange yang meledak ke segala arah
- **12 smoke particles**: Asap abu-abu yang bergerak lebih lambat
- **4 flash particles**: Efek kilau white/yellow yang cepat hilang
- **Physics:** Velocity friction (0.94x per frame), gravity tidak ada, rotation animation
- **Color Lerp:** Smooth transition dari start color ke end color

### 7. **Sound Effects & Music**
- 🎵 **Background Music:** Looping background musik ambient
- 🔊 **Sound Effects:**
  - Spongebob fail sound (saat game over)
  - "Fahhh" impact sound (juga saat game over)
  - Score milestone notification (setiap 1000 score)
  - Bone crack sound (saat terkena damage)
- Volume balancing yang profesional

### 8. **UI & Menu System**

**Main Menu Features:**
- Menampilkan high score sesi
- Pilih difficulty (Low/Medium/High)
- Pilih bullet color (jika unlocked)
- Visual indicator untuk bullet color yang terlocked
- Press Start 2P font untuk aesthetic retro

**Game Over Screen:**
- Overlay semi-transparent red
- Large "GAME OVER" text dengan glow effect
- Menampilkan final score dan high score
- Option: Retry atau Back to Menu
- Devil Breeze Bold font untuk drama effect

**In-Game HUD:**
- Real-time score counter
- High score display
- Health counter (X / 3)
- Positioned di top-left untuk clear view

### 9. **Game States**
Game memiliki 3 state utama:
1. **MainMenu** - Navigasi menu sebelum bermain
2. **Playing** - Gameplay aktif
3. **GameOver** - Screen setelah kalah

### 10. **Score System**
- **+10 points** untuk setiap obstacle yang dihancurkan
- **Score milestone sound** setiap 1000 points
- **Automatic high score tracking** sepanjang sesi
- **Bullet color unlock** tergantung high score dan difficulty yang dicapai

---

## 🎯 Gameplay

### Bagaimana Bermain?

1. **Mulai Game:** Tekan Enter pada "Play" di menu
2. **Atur Difficulty:** Pilih Cortisol level (Low/Medium/High)
3. **Atur Bullet Color:** Pilih warna peluru yang unlocked
4. **Main:** 
   - Gerakkan pesawat dengan WASD
   - Tembak dengan Space
   - Jangan biarkan obstacle lolos ke kiri
5. **Game Over:** Ketika health 0 atau tabrakan langsung

### Strategi Main:

- **Low Difficulty:** Bagus untuk tutorial dan unlock Blue bullet
- **Medium Difficulty:** Balanced challenge, unlock Red bullet di sini
- **High Difficulty:** Pro level, unlock Purple bullet yang powerful
- **Purple Bullet:** Gunakan di High difficulty untuk pierce obstacles yang datang beruntun

---

## 📊 Sistem Difficulty (Cortisol)

"Cortisol" dipilih sebagai nama difficulty karena hubungannya dengan stress level:

### Efek Tiap Level:

**🟢 Low**
```
Obstacle Speed: 3.0 px/frame (pelan)
Spawn Rate: 1.8 detik sekali
Max obstacles on screen: ~1 obstacle
Playstyle: Relaxed, dapat practice aim
```

**🟡 Medium**
```
Obstacle Speed: 5.5 px/frame (normal)
Spawn Rate: 1.0 detik sekali
Max obstacles on screen: ~5-6 obstacles
Playstyle: Balanced, skill-dependent
```

**🔴 High**
```
Obstacle Speed: 9.0 px/frame (cepat)
Spawn Rate: 0.45 detik sekali
Max obstacles on screen: ~20+ obstacles
Playstyle: Intense, reflexes required
```

---

## 🎨 Sistem Bullet Color

### Visual Design

**Basic (Kuning)**
- Simple circle shape
- Standar bullet speed
- Available dari awal

**Blue (Biru)**
- Triangle/spear design
- **35% faster** daripada standar (15.525 px/frame)
- Shoot delay lebih pendek (280ms vs 330ms)
- Outline color cyan

**Red (Merah)**
- Complex shape (top, middle, bottom bars + curves)
- Normal speed
- Outline color light red
- Aesthetic: Heavy, powerful look

**Purple (Ungu)**
- Rectangular core dengan circular ends
- **Pierce mechanic:** Bisa menembus 1 obstacle
- Medium shoot delay (380ms)
- Special glow effect dan highlight core
- Most powerful bullet type

### Unlock Progression

```
Session Start
    ↓
Bermain Low difficulty, score ≥ 1000
    ↓ Blue unlocked!
Bermain Medium difficulty, score ≥ 5000
    ↓ Red unlocked!
Bermain High difficulty, score ≥ 10000
    ↓ Purple unlocked!
All bullets available untuk sesi ini
```

---

## ⌨️ Kontrol Game

### In-Game Controls

| Tombol | Fungsi |
|--------|--------|
| **W** | Gerak atas |
| **A** | Gerak kiri |
| **S** | Gerak bawah |
| **D** | Gerak kanan |
| **Space** | Tembak |

### Menu Controls

| Tombol | Fungsi |
|--------|--------|
| **W** | Gerak ke opsi atas |
| **S** | Gerak ke opsi bawah |
| **Enter** | Pilih opsi |

### Game Over Controls

| Tombol | Fungsi |
|--------|--------|
| **W** / **S** | Pilih antara Retry atau Back to Menu |
| **Enter** | Confirm pilihan |
| **R** | Langsung Retry |
| **M** | Langsung Back to Menu |

---

##  Setup & Build

### Requirements

- **C++17** atau lebih baru
- **SFML 3.0** atau lebih baru
- **MinGW g++** (untuk Windows)
- **Visual Studio Code** dengan C/C++ extension

### Path Configuration

Default paths (ubah sesuai sistem Anda):
```
C:/SFML-3.0.2          - SFML library location
C:/mingw64/bin/g++.exe - MinGW compiler
```

### Quick Build (Windows PowerShell)

```powershell
C:/mingw64/bin/g++.exe -std=c++17 -g `
  SpaceShoott.cpp `
  Game.cpp `
  Background.cpp `
  Player.cpp `
  Bullet.cpp `
  Obstacle.cpp `
  Explosion.cpp `
  -IC:/SFML-3.0.2/include `
  -LC:/SFML-3.0.2/lib `
  -lsfml-graphics `
  -lsfml-window `
  -lsfml-system `
  -lsfml-audio `
  -o SpaceShoott.exe
```

### Run with VS Code

1. Buka folder project di VS Code
2. Go to **Run and Debug** (Ctrl+Shift+D)
3. Select **Run SpaceShoot**
4. Press **F5**

---

##  Struktur Project

```
SpaceShoot/
│
├── assets/
│   ├── bgcc.png                          # Main game background
│   ├── bgcc_blur.png                     # Blurred background untuk menu
│   ├── player_ship_pixel_right.png       # Player sprite
│   ├── batu.gif                          # Obstacle/asteroid sprite
│   ├── backsound.wav                     # Background music
│   ├── sounds/
│   │   ├── spongebob-fail.mp3            # Game over sound 1
│   │   ├── fahhh_KcgAXfs.mp3             # Game over sound 2
│   │   ├── fears-to-fathom-notification-sound.mp3  # Score milestone
│   │   └── bone-crack.mp3                # Damage sound
│   └── fonts/
│       ├── arial.ttf                     # Main font
│       ├── PressStart2P-Regular.ttf      # Menu font (retro)
│       └── devil_breeze/
│           └── Devil Breeze Bold.ttf     # Game Over font
│
├── .vscode/
│   ├── launch.json                       # VS Code debug config
│   ├── tasks.json                        # Build tasks
│   └── c_cpp_properties.json             # Include paths
│
├── SFML-DLL/                             # SFML DLL files
│
├── Source Files (.cpp)
│   ├── SpaceShoott.cpp                   # Main entry point
│   ├── Game.cpp                          # Game logic & loop
│   ├── Player.cpp                        # Player class
│   ├── Bullet.cpp                        # Bullet class
│   ├── Obstacle.cpp                      # Obstacle/Asteroid class
│   ├── Background.cpp                    # Scrolling background
│   └── Explosion.cpp                     # Particle explosion effects
│
├── Header Files (.hpp)
│   ├── Game.hpp
│   ├── Player.hpp
│   ├── Bullet.hpp
│   ├── Obstacle.hpp
│   ├── Background.hpp
│   ├── Explosion.hpp
│   └── Constants.hpp                     # Game constants
│
└── README.md                             # Project documentation
```

---

##  Technical Highlights

### 1. **Efficient Memory Management**
```cpp
// Smart pointers untuk automatic cleanup
std::unique_ptr<Background> background;
std::unique_ptr<Player> player;

// Dynamic containers
std::vector<Bullet> bullets;      // Bullets yang active
std::vector<Obstacle> obstacles;  // Obstacles yang active
std::vector<Explosion> explosions; // Explosions yang active
```

### 2. **Optimized Collision Detection**
```cpp
// 2-stage collision:
// 1. Bounding box collision (fast)
if(bullet.getBounds().findIntersection(obstacle.getBounds()).has_value())
{
    // 2. Pixel-perfect collision (accurate)
    if(bullet.overlapsOpaquePixel(obstacle))
    {
        // Collision confirmed!
    }
}
```

### 3. **Pixel-Perfect Detection**
- Checks 9 sample points per bullet (center + corners + edges)
- Hanya count opaque pixels (alpha > 30)
- Mencegah collision dengan transparent area
- Akurat tapi tetap performant

### 4. **Object Lifecycle Management**
```cpp
// Update phase
for(auto& bullet : bullets)
    bullet.update();

// Clean phase (remove inactive/offscreen)
bullets.erase(
    std::remove_if(bullets.begin(), bullets.end(), 
        [](const Bullet& b) { return !b.isActive() || b.isOffscreen(); }),
    bullets.end()
);
```

### 5. **Smooth Visual Effects**
**Scrolling Background**
- Dua sprite yang berjalan parallel
- Seamless looping
- Constant speed (1.0 px/frame)

**Particle Explosion**
- 42 particles per explosion
- Color interpolation (lerp) untuk smooth fade
- Velocity friction untuk natural deceleration
- Rotation animation per particle

### 6. **Audio Management**
```cpp
// Music looping
music.setLooping(true);
music.play();

// Volume balancing (0-100 scale)
spongebobFailSound.setVolume(90.f);   // Loud fail sound
scoreMilestoneSound.setVolume(80.f);  // Moderate notification
```

### 7. **Game State Pattern**
```cpp
enum class GameState {
    MainMenu,      // Menu navigation
    Playing,       // Active gameplay
    GameOver       // Loss state
};
```

### 8. **Random Number Generation**
```cpp
std::mt19937 rng(std::random_device{}());
std::uniform_real_distribution<float> obstacleY(0.f, WindowHeight - 100.f);

// Usage
float randomY = obstacleY(rng);  // Random Y position untuk obstacle
```

### 9. **Frame-Based Timing**
- Fixed 60 FPS
- Clock-based timing untuk shoot delay, spawn delay, animations
- Smooth frame rate independent gameplay

### 10. **Color & Visual Polish**
- Color-based bullet style detection
- RGB color matching untuk identify bullet type
- Custom color palettes untuk setiap element
- Outline thickness untuk depth effect

---

##  Asset & Dependency

### Required Assets

| File | Type | Purpose |
|------|------|---------|
| `bgcc.png` | Texture | Game background |
| `bgcc_blur.png` | Texture | Menu/GameOver background (blurred) |
| `player_ship_pixel_right.png` | Texture | Player sprite |
| `batu.gif` | Texture | Obstacle/asteroid sprite |
| `backsound.wav` | Audio | Background music |
| `spongebob-fail.mp3` | Audio | Game over sound |
| `fahhh_KcgAXfs.mp3` | Audio | Additional game over sound |
| `fears-to-fathom-notification-sound.mp3` | Audio | Score milestone sound |
| `bone-crack.mp3` | Audio | Damage/hurt sound |
| `arial.ttf` | Font | Game UI font |
| `PressStart2P-Regular.ttf` | Font | Menu font (retro style) |
| `Devil Breeze Bold.ttf` | Font | Game over screen font |

### SFML Libraries Used

```cpp
#include <SFML/Graphics.hpp>  // Sprites, shapes, text rendering
#include <SFML/Audio.hpp>     // Music dan sound effects
#include <SFML/Window.hpp>    // Window management (implicit)
#include <SFML/System.hpp>    // Clock, Vector math (implicit)
```

### C++ Standard Library

```cpp
#include <vector>          // Dynamic arrays (bullets, obstacles, explosions)
#include <memory>          // Smart pointers (unique_ptr)
#include <random>          // Random number generation
#include <algorithm>       // std::remove_if untuk cleanup
#include <string>          // Text/score display
#include <cmath>           // Math functions
#include <array>           // Fixed-size arrays (9 sample points)
```

---

## 🎓 Learning Points

### Untuk Programmer Pemula

1. **Game Loop Pattern** - Struktur update/render loop yang konsisten
2. **Object-Oriented Design** - Separate classes untuk Player, Bullet, Obstacle
3. **Vector & Container Management** - Efficient use of std::vector
4. **Collision Detection** - Simple bounding box + advanced pixel-perfect
5. **Game State Management** - Multiple states (Menu, Playing, GameOver)
6. **Event Handling** - Keyboard input dan window events

### Untuk Game Developer

1. **Performance Optimization** - 2-stage collision detection
2. **Particle System** - Explosion effects dengan physics
3. **Audio Management** - Volume balancing dan effect layering
4. **UI/UX Design** - Menu structure dan in-game feedback
5. **Visual Polish** - Color schemes, fonts, animations
6. **Game Feel** - Sound effects timing, visual feedback (damage flash)

---

##  Future Enhancements

Potential improvements untuk versi selanjutnya:

- [ ] **Leaderboard** - Save high scores to file
- [ ] **Power-ups** - Shield, rapid fire, slow motion
- [ ] **Enemy Variety** - Different obstacle types
- [ ] **Levels** - Progressive difficulty
- [ ] **Achievements** - Badges dan tracking
- [ ] **Sound Options** - Volume slider, mute button
- [ ] **Pause Feature** - Pause during gameplay
- [ ] **Tutorial** - In-game tutorial screen
- [ ] **Mobile Controls** - Touch input support
- [ ] **Multiplayer** - Local or online modes

---

##  Credits

**Developed by:** Dzaky Haady - 5024241076  
**Engine:** SFML 3.0  
**Language:** C++17  



**Semoga dari game ini sukses**
