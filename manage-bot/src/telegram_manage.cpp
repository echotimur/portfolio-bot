#include "telegram_manage.hpp"
#include "cli_manage.hpp"

#include <string>
#include <tgbot/tgbot.h>
#include <vector>


TGManage::TGManage(const std::string& apikey) : bot(apikey) {}


void TGManage::startTGBot() {

    TgBot::InlineKeyboardMarkup::Ptr keyboard_one(new TgBot::InlineKeyboardMarkup());
    std::vector<TgBot::InlineKeyboardButton::Ptr> rw0;
    TgBot::InlineKeyboardButton::Ptr portfolioButton(new TgBot::InlineKeyboardButton);
    portfolioButton->text = "Портфель";
    portfolioButton->callbackData = "portfolio_query";

    rw0.push_back(portfolioButton);
    keyboard_one->inlineKeyboard.push_back(rw0);

    bot.getEvents().onCommand("start", [&](TgBot::Message::Ptr message){

        bot.getApi().sendMessage(message->chat->id, "Покажем портфель", nullptr, nullptr, keyboard_one);

    });

    bot.getEvents().onCallbackQuery([&](TgBot::CallbackQuery::Ptr query){

        if(query->data == "portfolio_query"){

            CLIManage cli("./portfolio");

            bot.getApi().deleteMessage(query->message->chat->id, query->message->messageId);

            bot.getApi().sendMessage(query->message->chat->id, cli.getCommandOutput(), nullptr, nullptr, keyboard_one);

        }

    });


    TgBot::TgLongPoll longPoll(bot);

    int count = 0;

    while(true){
        if(count == 0){
            std::cout << "Long poll started\n";
            count = 1000;
        }
        longPoll.start();
        count--;
    }

}