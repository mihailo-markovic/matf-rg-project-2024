//
// Created by grafika on 1/21/26.
//

#ifndef MATF_RG_PROJECT_MAINCONTROLLER_HPP
#define MATF_RG_PROJECT_MAINCONTROLLER_HPP
#include <engine/core/App.hpp>
#include <engine/core/Controller.hpp>

namespace app {
class MainController : public engine::core::Controller {
    void initialize() override;

    bool loop() override;

public:
    std::string_view name() const override { return "app::MainController"; }

};
}// MainController

#endif //MATF_RG_PROJECT_MAINCONTROLLER_HPP