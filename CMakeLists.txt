# Определение функции HexVersion прямо в файле для надежности сборки
macro(HexVersion var major minor patch)
    math(EXPR ${var} "(${major} << 16) | (${minor} << 8) | ${patch}")
endmacro()
