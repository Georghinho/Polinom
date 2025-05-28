    #include "polinom.h"
    #include <gtest.h>

    TEST(Monom, CanCreateWithUnaryMinus) {
        ASSERT_NO_THROW(Monom m("-2x^6y^7z^8"));
    }

    TEST(Monom, CanCreateStandard) {
        ASSERT_NO_THROW(Monom m("2x^6y^7z^8"));
    }

    TEST(Monom, AdditionWorksCorrectly) {
        Monom m1("2x^6y^7z^8");
        Monom m2("2x^6y^7z^8");
        Monom expected("4x^6y^7z^8");
        EXPECT_EQ(m1 + m2, expected);
    }

    TEST(Monom, ThrowsWhenAddingDifferentDegrees) {
        Monom m1("2x^4y^7z^8");
        Monom m2("2x^6y^7z^8");
        EXPECT_ANY_THROW(m1 + m2);
    }

    TEST(Monom, SubtractionWorksCorrectly) {
        Monom m1("6x^6y^7z^8");
        Monom m2("2x^6y^7z^8");
        Monom expected("4x^6y^7z^8");
        EXPECT_EQ(m1 - m2, expected);
    }

    TEST(Monom, ScalarMultiplicationWorks) {
        Monom m("2x^4y^7z^8");
        Monom expected("6x^4y^7z^8");
        EXPECT_EQ(m * 3, expected);
    }

    TEST(Monom, MultiplicationByZeroGivesZero) {
        Monom m("2x^4y^7z^8");
        Monom expected("0");
        EXPECT_EQ(m * 0, expected);
    }

    TEST(Monom, MonomMultiplicationWorks) {
        Monom m1("2x^4y^7z^5");
        Monom m2("3x^4y^2z^2");
        Monom expected("6x^8y^9z^7");
        EXPECT_EQ(m1 * m2, expected);
    }

    TEST(Monom, ThrowsOnDegreeOverflow) {
        Monom m1("2x^4y^7z^5");
        Monom m2("3x^9y^2z^2");
        EXPECT_ANY_THROW(m1 * m2);
    }

    TEST(Polinom, CanCreateFromString) {
        ASSERT_NO_THROW(Polinom p("2x^6y^7z^8+3x^5y^4z^3"));
    }

    TEST(Polinom, CorrectlyParsesAndCombinesLikeTerms) {
        Polinom p("2x^6y^7z^8+2x^6y^7z^8+2x^6y^7z^8");
    
        EXPECT_EQ(p.size(), 1);
        if (!p.getMonoms().empty()) {
            EXPECT_DOUBLE_EQ(p.getMonoms()[0].coeff, 6.0); 
            EXPECT_EQ(p.getMonoms()[0].degree, 678); 
        }
    }
    TEST(Polinom, AdditionWithDifferentTerms) {
        Polinom p1("4x^4y^3z^2+8x^5y^3z^2");
        Polinom p2("6x^9y^3z^2");
        Polinom expected("6x^9y^3z^2+8x^5y^3z^2+4x^4y^3z^2");
        EXPECT_EQ(p1 + p2, expected);
    }

    TEST(Polinom, CombinesLikeTermsOnAddition) {
        Polinom p1("4x^4y^3z^2");
        Polinom p2("6x^4y^3z^2");
        Polinom expected("10x^4y^3z^2");
        EXPECT_EQ(p1 + p2, expected);
    }

    TEST(Polinom, SubtractionWithDifferentTerms) {
        Polinom p1("4x^4y^3z^2+8x^5y^3z^2");
        Polinom p2("6x^9y^3z^2");
        Polinom expected("-6x^9y^3z^2+8x^5y^3z^2+4x^4y^3z^2");
        EXPECT_EQ(p1 - p2, expected);
    }

    TEST(Polinom, MultiplicationWorksCorrectly) {
        Polinom p1("4x^4y^3z^2");
        Polinom p2("6x^4y^3z^2");
        Polinom expected("24x^8y^6z^4");
        EXPECT_EQ(p1 * p2, expected);
    }

    TEST(Polinom, ComplexMultiplication) {
        Polinom p1("x^3+x^1+1");
        Polinom p2("x^2+1");
        Polinom expected("x^5+2x^3+x^2+x^1+1");
        EXPECT_EQ(p1 * p2, expected);
    }

    TEST(Polinom, HandlesZeroPolinom) {
        Polinom p1("3x^2y^3z^4");
        Polinom p2("-3x^2y^3z^4");
        Polinom result = p1 + p2;
        EXPECT_TRUE(result.getMonoms().empty()); 
    }

    TEST(Polinom, MaintainsCorrectOrdering) {
        Polinom p("x^3y^2z^1+x^1y^5z^9+x^9y^0z^2");
        std::vector<int> expected_degrees = { 902, 321, 159 };
        std::vector<int> actual_degrees;

    
        const auto& monoms = p.getMonoms();
        for (const auto& m : monoms) {
            actual_degrees.push_back(m.degree);
        }

        EXPECT_EQ(expected_degrees, actual_degrees);
    }

    TEST(Polinom, HandlesFloatingPointCoefficients) {
        Polinom p1("2.5x^3y^2z^1");
        Polinom p2("1.5x^3y^2z^1");
        Polinom sum = p1 + p2;

    
        ASSERT_FALSE(sum.getMonoms().empty());

    
        const Monom& result = sum.getMonoms()[0];

    
        EXPECT_NEAR(result.coeff, 4.0, 1e-10);

   
        EXPECT_EQ(result.degree, 321); 
    }

    TEST(Polinom, CopyOperationsWork) {
        Polinom p1("3x^2y^3z^4+2x^5y^1z^2");
        Polinom p2(p1);
        Polinom p3 = p1;
        EXPECT_EQ(p1, p2);
        EXPECT_EQ(p1, p3);
    }

    TEST(Polinom, IgnoresWhitespace) {
        Polinom p1(" 3x^2y^3z^4 + 2x^5y^1z^2 ");
        Polinom p2("3x^2y^3z^4+2x^5y^1z^2");
        EXPECT_EQ(p1, p2);
    }

    TEST(Polinom, InequalityDetection) {
        Polinom p1("3x^2y^3z^4");
        Polinom p2("3x^2y^3z^5");
        EXPECT_NE(p1, p2);
    }

    TEST(Polinom, EmptyPolinomAddition) {
        Polinom p1("3x^2y^3z^4");
        Polinom p2("");
        EXPECT_EQ(p1 + p2, p1);
    }

    TEST(Polinom, RegressionTest1) {
        Polinom p1("x^3+x^1+1");
        Polinom p2("x^2+1");
        Polinom expected("x^5+2x^3+x^2+x^1+1");
        EXPECT_EQ(p1 * p2, expected);
    }

    TEST(Polinom, RegressionTest2) {
        Polinom p1("x^3+x^1+1");
        Polinom p2("x^2+1");
        Polinom expected("x^5+2x^3+x^2+x^1+1");
        EXPECT_EQ(p1 * p2, expected);
    }

    TEST(Monom, Abc) {
        Monom p("xyz");
        int degree = 111;
        EXPECT_EQ(p.degree, degree);

    }