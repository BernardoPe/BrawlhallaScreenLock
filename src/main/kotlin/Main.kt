import com.sun.jna.*
import javax.swing.*
import java.awt.*
import java.util.concurrent.atomic.AtomicBoolean

const val WINDOW_CLASS = "ApolloRuntimeContentWindow"
const val MOUSE_POSITION_BUFFER = 50
const val WINDOW_UPDATE_INTERVAL_MS = 100L

val user32: User32 = Native.load("user32", User32::class.java)

fun main() {
    SwingUtilities.invokeLater { createAndShowGUI() }
}

fun createAndShowGUI() {
    JFrame("Cursor Lock for Brawlhalla").apply {
        defaultCloseOperation = JFrame.EXIT_ON_CLOSE
        setSize(350, 120)
        layout = BorderLayout()

        val statusLabel = JLabel("Status: Idle")
        val toggleButton = JButton("Start Lock")
        val isRunning = AtomicBoolean(false)

        toggleButton.addActionListener {
            if (isRunning.compareAndSet(false, true)) {
                toggleButton.text = "Stop Lock"
                statusLabel.text = "Status: Running"
                Thread { lockCursorLoop(isRunning, statusLabel) }.apply { start() }
            } else {
                isRunning.set(false)
                toggleButton.text = "Start Lock"
                statusLabel.text = "Status: Idle"
            }
        }

        add(statusLabel, BorderLayout.NORTH)
        add(toggleButton, BorderLayout.CENTER)
        isVisible = true
    }
}

fun lockCursorLoop(isRunning: AtomicBoolean, statusLabel: JLabel) {
    var lastUpdateTime = 0L
    var lockedWindow: HWND? = null

    while (isRunning.get()) {
        val currentWindow = user32.GetForegroundWindow()

        if (lockedWindow == null || currentWindow == null || currentWindow != lockedWindow) {
            if (System.currentTimeMillis() - lastUpdateTime > WINDOW_UPDATE_INTERVAL_MS) {
                lockedWindow = user32.FindWindowA(WINDOW_CLASS, null)
                lastUpdateTime = System.currentTimeMillis()
            }
            Thread.sleep(10)
            continue
        }

        val windowRect = RECT()
        if (!user32.GetWindowRect(lockedWindow, windowRect)) {
            Thread.sleep(100)
            continue
        }

        val cursorPos = POINT()
        if (!user32.GetCursorPos(cursorPos)) {
            Thread.sleep(100)
            continue
        }

        val clampedX = cursorPos.x.coerceIn(windowRect.left + MOUSE_POSITION_BUFFER, windowRect.right - MOUSE_POSITION_BUFFER)
        val clampedY = cursorPos.y.coerceIn(windowRect.top + MOUSE_POSITION_BUFFER, windowRect.bottom - MOUSE_POSITION_BUFFER)

        if (clampedX != cursorPos.x || clampedY != cursorPos.y) {
            user32.SetCursorPos(clampedX, clampedY)
        }
        Thread.sleep(10)
    }

    statusLabel.text = "Status: Idle"
}

typealias HWND = Pointer
