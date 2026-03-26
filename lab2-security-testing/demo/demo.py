from typing import Tuple

class BankAccount:
    
    def __init__(self, owner: str, account_number: Tuple[int,int,int], initial_balance:int = 0) -> None:
        self.owner = owner
        self.account_number = account_number
        self.balance = initial_balance
    
    def deposit(self, amount):
        self.balance += amount
    
    def withdraw(self, amount):
        self.balance -= amount
    
    def is_overdrawn(self):
        return self.balance < 0
    
    def __str__(self):
        return f"Balance of {self.owner} (account number: {self.account_number}) is: {self.balance}"


account = BankAccount("Dagobert Duck", (12345678,87654321,00000000), 15)
account.withdraw(5)
print(account)

# wrong function call
#account.deposit("test")