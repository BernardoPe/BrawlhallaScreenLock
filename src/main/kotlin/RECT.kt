import com.sun.jna.Structure

class RECT : Structure() {
    @JvmField var left: Int = 0
    @JvmField var top: Int = 0
    @JvmField var right: Int = 0
    @JvmField var bottom: Int = 0

    override fun getFieldOrder() = listOf("left", "top", "right", "bottom")
}
