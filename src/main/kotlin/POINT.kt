import com.sun.jna.Structure

class POINT : Structure() {
    @JvmField var x: Int = 0
    @JvmField var y: Int = 0

    override fun getFieldOrder() = listOf("x", "y")
}