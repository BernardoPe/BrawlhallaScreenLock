# Brawlhalla Screen Lock

Brawlhalla does not have exclusive fullscreen, meaning that the cursor can leave the game window and
interact with the desktop. This can be annoying during gameplay, so this tool locks the cursor
to the game window, preventing it from leaving the game area.

# Build and Run

To build and run the project, run the following commands in the root directory of the project:

```bash
./gradlew fatJar

java -jar build/libs/BrawlhallaScreenLock-1.0-SNAPSHOT-all.jar
```