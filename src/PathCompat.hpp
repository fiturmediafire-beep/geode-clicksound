#pragma once
// Jembatan antara asp::fs::Path (Geode v6) dan std::filesystem::path.
// Taruh di src/PathCompat.hpp. Kalau ada error konversi, cukup sesuaikan file ini.

#include <Geode/Geode.hpp>
#include <concepts>
#include <filesystem>
#include <string>

namespace cs::pathcompat {

    // Path (asp::fs::Path / PathView) -> std::string (UTF-8).
    // Mencoba beberapa nama method yang umum, mana yang ada akan dipakai.
    template <class P>
    inline std::string toUtf8(P const& p) {
        if constexpr (requires { std::string(p.toString()); }) {
            return std::string(p.toString());
        } else if constexpr (requires { std::string(p.string()); }) {
            return std::string(p.string());
        } else {
            return fmt::format("{}", p);
        }
    }

    // std::string (UTF-8) -> std::filesystem::path
    inline std::filesystem::path toStd(std::string const& s) {
        return std::filesystem::path(std::u8string(s.begin(), s.end()));
    }

    // asp::fs::Path / PathView -> std::filesystem::path
    template <class P>
    inline std::filesystem::path toStd(P const& p) {
        return toStd(toUtf8(p));
    }

    // std::filesystem::path -> asp::fs::Path
    inline asp::fs::Path fromStd(std::filesystem::path const& p) {
        auto u = p.u8string();
        return asp::fs::Path(std::string(u.begin(), u.end()));
    }

} // namespace cs::pathcompat
