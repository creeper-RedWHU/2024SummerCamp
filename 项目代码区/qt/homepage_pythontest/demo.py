def check_parameters(string_param, int1, int2, int3, int4):
    if not isinstance(string_param, str):
        return "Error: The first parameter must be a string."

    if not all(isinstance(i, int) for i in [int1, int2, int3, int4]):
        return "Error: The last four parameters must be integers."

    return "Parameters are correctly passed."

# 测试函数
result = check_parameters("example", 1, 2, 3, 4)
print(result)  # 输出: Parameters are correctly passed.

result = check_parameters(123, 1, 2, 3, 4)
print(result)  # 输出: Error: The first parameter must be a string.

result = check_parameters("example", 1, 2, "three", 4)
print(result)  # 输出: Error: The last four parameters must be integers.
