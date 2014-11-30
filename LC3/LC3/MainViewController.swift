//
//  MainViewController.swift
//  LC3
//
//  Created by Brendan Henderson on 11/30/14.
//  Copyright (c) 2014 Brendan Henderson. All rights reserved.
//

import Foundation
import UIKit

extension UIView {
    var width:CGFloat{
        set {
            self.frame.size.width = newValue
        }
        get {
            return self.frame.size.width
        }
    }
}

class MainViewController : UIViewController {
    var isConsoleOpen = false
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        //swiping right opens the Console.
        var swipeToOpenConsole = UISwipeGestureRecognizer(target: self, action: "openConsole")
        swipeToOpenConsole.direction = UISwipeGestureRecognizerDirection.Right
        consoleView.addGestureRecognizer(swipeToOpenConsole)
        //swiping left closes the Console.
        var swipeToCloseConsole = UISwipeGestureRecognizer(target: self, action: "closeConsole")
        swipeToCloseConsole.direction = UISwipeGestureRecognizerDirection.Left
        consoleView.addGestureRecognizer(swipeToCloseConsole)
    }
    
    func openConsole() {
        isConsoleOpen = true
        UIView.animateWithDuration(1, animations: { () -> Void in
            self.consoleView.width = self.view.width - 25
            self.consoleView.backgroundColor=UIColor.blueColor()
        })
    }
    func closeConsole() {
        isConsoleOpen = false
        UIView.animateWithDuration(1, animations: { () -> Void in
            self.consoleView.width = 25
            self.consoleView.backgroundColor=UIColor.blackColor()
        })
    }
    @IBAction func runClicked(sender: AnyObject) {
        if isConsoleOpen {
            closeConsole()
        } else {
            openConsole()
        }
    }
    
    @IBOutlet weak var consoleView: UIView!
}