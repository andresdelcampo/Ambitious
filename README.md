# Ambitious v10
Over 20+ years ago I embarked into the educational adventure of learning how Microsoft's 25 letter product key system worked (before it used Internet to activate). The result of that work was a C++ Builder application back in 2001. I have decided to resurrect this old project back to life in Visual Studio 2022 / .NET Framework 4.7.2 and share it with the community -strictly for educational purposes.

This application can:
* Evaluate if a product key for old Microsoft products is valid. 
* Show you the different components / data included in the key
* Let you create your own key system and generate your own keys for it -in fact I used to have a license to this program created by this program!
* Help you learn -together with the sources- the inner workings of the 25-product-key algorithm
  
**Disclaimer:** It will **NOT** generate you valid product keys unless you can enter Microsoft private keys used to generate product keys -which I never had and to my knowledge Microsoft never published. Therefore, it cannot be used to pirate software. Furthermore, all products targeted are already far beyond their "End of Life" date. 

Ambitious consists of two projects:
* AmbitiousLib: The main library project in C++ that contains the product key algorithms -including elliptic curves and SHA-1
* Ambitious: The C# frontend project in .NET 4.7.2. 

## Product keys it recognizes
* Windows 98 first and second edition
* Windows Me
* Windows 2000 (multiple versions)
* Windows XP Pro
* Office 2000 Full and upgrade
* Office XP Pro
* Visual Studio .NET (first version)
* IE5 Admin kit

## Briefly about product keys
Microsoft 25 character product keys are fascinating. Had I known how complex they were, I would not have started this project back then. 

They are basically an **incredibly HUGE number coded in base 24** (so 25 to the power of 24!!). Why base 24? Because it can only contain any of these 24 characters/numbers: BCDFGHJKMPQRTVWXY2346789. It was avoiding digits and letters that could be confused like 1 and L, or 5 and E. 

Product keys are generated using an Elliptic Curve Digital Signature Algorithm (ECDSA) and a Secure Hash Algorithm (SHA-1) -which are topics strongly recommended to learn about if you are trying to understand how these keys work. Public keys (required to validate the keys) can be found inside the target product files, while private keys (also required to create new keys) are only known to Microsoft -though you can create your own key systems with your own private and public keys with this application -for your own software. 

What was particularly puzzling when trying to find a way to generate new keys, is that SHA-1 is an irreversible algorithm -it reduces a big number into a smaller signature -and yet that irreversible signature is compared back to part of the source product key that was used to calculate that signature in the first place!
