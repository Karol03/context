/**
 * Created by Karol Dudzic @ 2022
 */
#include <bits/stdc++.h>

#include "ctx/context.hpp"


void function(ctx::Context& context);
void constFunction(const ctx::Context& context);


int main()
{

    ctx::Context context =
            ctx::Context::create<"first", "second", "third", "fourth">();   // create context to store
                                                                            // four elements

    int* first = new int(52);           // first param is raw pointer
    const double second = 52.25L;       // second is const double
    std::string third = "Third param";  // third is string

    ctx::Context::Builder{context}      // use builder is equivalent to context.set(..name..., ..value...)
        .set("first", first)            // first param as int*, do not delete pointer by yourself
                                        //      context will take care of it
        .set("second", second)          // second param as const double&
        .set("third", std::move(third)) // third param as std::string, context will store it
        .set("fourth", "fourth param"); // fourth param as const char*

    // use context in the function
    function(context);
    // and in const function
    constFunction(context);

    return 0;
}

void function(ctx::Context& context)    // context copy-constructor is delted!
{
    auto* first = context.get<float>("first");       // get first param as float
                                                     // it will fail, cause first param is int
    if (!first)
    {
        std::cerr << "First param is not a float!\n";
    }

    auto* ifirst = context.get<int>("first");       // get first param as int
                                                    // it will success cause first param has int value
    if (ifirst)
    {
        std::cerr << "First param is int! Value = " << *ifirst << "\n";
    }

    auto* second = context.get<double>("second");   // get second param as double
                                                    // it will fail cause second param is const
    if (!second)
    {
        std::cerr << "Second param is not mutable!\n";
    }

    const auto* csecond = context.get_const<double>("second");   // get second param as const double
                                                                 // it will success
    if (csecond)
    {
        std::cerr << "Second param is double! Value = " << *csecond << "\n";
    }
}

void constFunction(const ctx::Context& context)
{
    if (const auto* first = context.get<int>("first"))    // get first param as const int
                                                          // it will success cause first param has int value
    {
        std::cerr << "First param is int! Value = " << *first << "\n";
    }

    if (const auto* second = context.get<double>("second"))  // get second param as const double
                                                             // it will success
    {
        std::cerr << "Second param is double! Value = " << *second << "\n";
    }
}
