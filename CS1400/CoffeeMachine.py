'''
Program Name: Coffee Machine
Programmed By: Joshua Krueger
Last Modified Date: 27/02/2021

Overall Program Desc.
    This program simulates a coffee machine allowing the user to insert coins to
    buy products.

Function and Class Descs.
    CoffeeMachine: The out most class the puts everything together
    CashBox: Manages the money
    Selector: Works with CashBox and manages Products
    Product: Abstraction of a product...
'''


class CoffeeMachine:
    '''
        Holds the selector and cashbox
        Does the loop logic for accepting coins and user interaction
    '''
    def __init__(self):
        self.cashBox = CashBox()
        self.selector = Selector(self.cashBox)
    
    def one_action(self):
        '''
        Handles user interaction and delegates actions
        Returns true until quit is entered
        '''
        ACTIONS = ['insert', 'select', 'cancel', 'quit']
        ACCEPTED_COINS = [50, 25, 10, 5]
        INTRO = (f'--------------------------------------------------\n'
                 f'\tPRODUCT LIST: all 35 cents, except bouillon (25 cents)\n'
                 f'\tSample commands: insert 25, select 1.\n'
                 f'>>> Your command: ')

        
        raw = ''
        validInput = False
        
        while(not validInput):
            raw = input(INTRO)

            num_Actions = 0

            for action in ACTIONS:
                if action in raw:
                    num_Actions += 1

            if num_Actions == 0:
                print('No action selected')
                continue
            elif num_Actions > 1:
                print('Too many actions')
                continue
        
            if ACTIONS[3] in raw:
                return False
            elif ACTIONS[2] in raw:
                self.selector.cashBox.returnCoins()
                continue
            elif ACTIONS[1] in raw:
                option = raw.split()[1]

                if option.isdigit():
                    option = int(option)

                    if option <= 0 or option > len(self.selector.products):
                        print('Invalid index')
                        continue
                    else:
                        self.selector.select(option)
                        continue
                else:
                    print('Invalid selection')
                    continue
            elif ACTIONS[0] in raw:
                coin = raw.split()[1]

                if coin.isdigit():
                    coin = int(coin)

                    if coin not in ACCEPTED_COINS:
                        print('Invalid amount')
                        continue

                    self.selector.cashBox.deposit(coin)
                else:
                    print('Invalid insert')
                    continue
            
            validInput = True
        
        return True

    def totalCash(self):
        '''
        Returns the total cash spent
        '''
        return self.selector.cashBox.total()


class CashBox:
    def __init__(self):
        self.credit = 0
        self.totalReceived = 0
    
    def deposit(self, amount):
        '''
        Takes an amount and stores it
        '''
        self.credit += amount
        self.totalReceived += amount

        print(f'Depositing {amount} cents. You have {self.credit} cents credit.')
    
    def returnCoins(self):
        '''
        Conceptually returns coins to user
        '''
        moneyToReturn = self.credit
        self.totalReceived -= moneyToReturn
        self.credit = 0
        
        if not moneyToReturn == 0:
            print(f'Returning {moneyToReturn} cents.')
    
    def haveYou(self, amount):
        '''
        Tests if there are enough credits for the purchase (amount)
        '''
        return False if amount > self.credit else True
    
    def deduct(self, amount):
        '''
        Deducts an amount from the credits
        '''
        self.credit -= amount
    
    def total(self):
        '''
        Returns the total spent money
        '''
        return self.totalReceived


class Selector:
    '''
    Coperates with cashbox to select products
    '''
    def __init__(self, _cashBox):
        self.cashBox = _cashBox
        self.products = [
            Product('black', 35, ['cup', 'coffee', 'water']),
            Product('white', 35, ['cup', 'coffee', 'creamer', 'water']),
            Product('sweet', 35, ['cup', 'coffee', 'sugar', 'water']),
            Product('white & sweet', 35, ['cup', 'coffee', 'sugar', 'creamer' 'water']),
            Product('bouillon', 25, ['cup', 'bouillon powder', 'water'])
            ]
    
    def select(self, choiceIndex):
        '''
        Takes in an index for the product the user wants
        and asks cashbox if they have enough credit
        if they do - the product is produced
        else - print error
        '''
        if not self.cashBox.haveYou(self.products[choiceIndex - 1].getPrice()):
            print('Not enough credit')
        else:
            self.cashBox.deduct(self.products[choiceIndex - 1].getPrice())
            self.products[choiceIndex - 1].make()
            self.cashBox.returnCoins()


class Product:
    def __init__(self, _name, _price, _recipe):
        self.name = _name
        self.price = _price
        self.recipe = _recipe
    
    def getPrice(self):
        '''
        Solves the Collatz Conjecture XD
        '''
        return self.price
    
    def make(self):
        '''
        Conceptually makes the product
        '''
        print(f'Making {self.name}:')

        for ingredient in self.recipe:
            print(f'\tDispensing {ingredient}')


def main():
    '''
    Creates a coffeemachine instance and calls one_action
    Prints the total cash spent
    '''
    m = CoffeeMachine()

    while m.one_action():
        pass

    print(f'Total cash: ${m.totalCash() / 100:.2f}')


if __name__ == '__main__':
    main()
