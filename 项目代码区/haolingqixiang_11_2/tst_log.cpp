#include <gtest/gtest.h>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QApplication>
#include "login_register.h"

    // Mock Database Connection Class
    class MockDatabase {
public:
    MockDatabase() {
        db = QSqlDatabase::addDatabase("QMYSQL");
        db.setHostName("60.205.232.122");  // 修改为你的数据库主机名
        db.setPort(3306);                  // 修改为你的数据库端口
        db.setDatabaseName("data");        // 修改为你的数据库名
        db.setUserName("root");            // 修改为你的数据库用户名
        db.setPassword("QAZ123wsx");  // 修改为你的数据库密码

        if (!db.open()) {
            qDebug() << "Error: connection with database fail";
        } else {
            qDebug() << "Database: connection ok";
        }

        QSqlQuery query;
        query.exec("CREATE TABLE IF NOT EXISTS users (account TEXT PRIMARY KEY, password TEXT, identity INTEGER)");
    }

    ~MockDatabase() {
        db.close();
    }

    void addUser(const QString &username, const QString &password, int identity) {
        QSqlQuery query;
        query.prepare("INSERT INTO users (account, password, identity) VALUES (:username, :password, :identity)");
        query.bindValue(":username", username);
        query.bindValue(":password", password);
        query.bindValue(":identity", identity);
        if (!query.exec()) {
            qDebug() << "Error adding user:" << query.lastError();
        }
    }

private:
    QSqlDatabase db;
};

class LoginRegisterTest : public ::testing::Test {
protected:
    static void SetUpTestSuite() {
        // Initialize the QApplication instance
        int argc = 0;
        char **argv = nullptr;
        app = new QApplication(argc, argv);
    }

    static void TearDownTestSuite() {
        delete app;
    }

    void SetUp() override {
        // Initialize the mock database
        mockDatabase = new MockDatabase();
        loginRegister = new login_register(nullptr);

        // Add a test user
        mockDatabase->addUser("testuser", "testpassword", 1);
    }

    void TearDown() override {
        delete loginRegister;
        delete mockDatabase;
    }

    login_register *loginRegister;
    MockDatabase *mockDatabase;

private:
    static QApplication *app;
};

QApplication* LoginRegisterTest::app = nullptr;

TEST_F(LoginRegisterTest, AuthenticateSuccess) {
    int identity;
    bool result = loginRegister->authenticate("testuser", "testpassword", identity);
    EXPECT_TRUE(result);
    EXPECT_EQ(identity, 1);
}

TEST_F(LoginRegisterTest, AuthenticateFail) {
    int identity;
    bool result = loginRegister->authenticate("wronguser", "wrongpassword", identity);
    EXPECT_FALSE(result);
}

TEST_F(LoginRegisterTest, UserExists) {
    bool result = loginRegister->userExists("testuser");
    EXPECT_TRUE(result);

    result = loginRegister->userExists("nonexistentuser");
    EXPECT_FALSE(result);
}

TEST_F(LoginRegisterTest, SaveUser) {
    loginRegister->saveUser("newuser", "newpassword");
    bool result = loginRegister->userExists("newuser");
    EXPECT_TRUE(result);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
