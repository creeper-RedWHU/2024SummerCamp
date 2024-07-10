#include <gtest/gtest.h>
#include <QApplication>
#include "login_register.h"

// 全局变量 argc 和 argv，在 main 函数中初始化 QApplication 时会用到
int argc = 1;
char *argv[] = {"LoginRegisterTest"};

// 测试登录功能
TEST(LoginRegisterTest, TestLogin) {
    QApplication a(argc, argv); // 创建 QApplication 对象

    login_register lr;

    // 测试正确的用户名和密码
    QString correctUsername = "1";
    QString correctPassword = "1";
    int identity = 1; // 假设身份的默认值

    // 使用正确的用户名和密码进行登录
    bool authenticated = lr.authenticate(correctUsername, correctPassword, identity);

    // 预期认证成功
    EXPECT_TRUE(authenticated);
    // 预期身份为1
    EXPECT_EQ(identity, 1);
}

// 测试注册功能
TEST(LoginRegisterTest, TestRegister) {
    QApplication a(argc, argv); // 创建 QApplication 对象

    login_register lr;

    // 测试注册一个新用户
    QString newUsername = "1234";
    QString newPassword = "1234";

    // 注册新用户
    lr.saveUser(newUsername, newPassword);

    // 验证新用户是否已存在
    bool userExists = lr.userExists(newUsername);

    // 预期新用户应该存在
    EXPECT_TRUE(userExists);
}
