/* Main ircd functions.
 *
 * (C) 2003-2008 Anope Team
 * Contact us at info@anope.org
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Epona by Lara.
 * Based on the original code of Services by Andy Church.
 *
 * $Id$
 *
 */

#include "services.h"
#include "extern.h"

IRCDProto *ircdproto;
IRCDModes ircd_modes;

/**
 * Globals we want from the protocol file
 **/
IRCDVar *ircd;
IRCDCAPAB *ircdcap;
char *version_protocol;
CBModeInfo *cbmodeinfos;
CUMode cumodes[128];
char *flood_mode_char_set;
char *flood_mode_char_remove;
CBMode cbmodes[128];
CMMode cmmodes[128];
char csmodes[128];
int UseTSMODE;

void pmodule_ircd_proto(IRCDProto *proto)
{
	ircdproto = proto;
}

void anope_set_umode(User *user, int ac, const char **av)
{
	ircdproto->set_umode(user, ac, av);
}

void anope_cmd_svsnoop(const char *server, int set)
{
	ircdproto->cmd_svsnoop(server, set);
}

void anope_cmd_remove_akill(const char *user, const char *host)
{
	ircdproto->cmd_remove_akill(user, host);
}

void anope_cmd_topic(const char *whosets, const char *chan, const char *whosetit, const char *topic, time_t when)
{
	ircdproto->cmd_topic(whosets, chan, whosetit, topic, when);
}

void anope_cmd_vhost_off(User *u)
{
	ircdproto->cmd_vhost_off(u);
}

void anope_cmd_akill(const char *user, const char *host, const char *who, time_t when, time_t expires, const char *reason)
{
	ircdproto->cmd_akill(user, host, who, when, expires, reason);
}

void anope_cmd_svskill(const char *source, const char *user, const char *fmt, ...)
{
	va_list args;
	char buf[BUFSIZE] = "";
	if (fmt) {
		va_start(args, fmt);
		vsnprintf(buf, BUFSIZE - 1, fmt, args);
		va_end(args);
	}
	ircdproto->cmd_svskill(source, user, buf);
}

void anope_cmd_svsmode(User *u, int ac, const char **av)
{
	ircdproto->cmd_svsmode(u, ac, av);
}

void anope_cmd_guest_nick(const char *nick, const char *user, const char *host, const char *real, const char *modes)
{
	ircdproto->cmd_guest_nick(nick, user, host, real, modes);
}

void anope_cmd_mode(const char *source, const char *dest, const char *fmt, ...)
{
	va_list args;
	char buf[BUFSIZE] = "";
	if (fmt) {
		va_start(args, fmt);
		vsnprintf(buf, BUFSIZE - 1, fmt, args);
		va_end(args);
	}
	ircdproto->cmd_mode(source, dest, buf);
}

void anope_cmd_bot_nick(const char *nick, const char *user, const char *host, const char *real, const char *modes)
{
	ircdproto->cmd_bot_nick(nick, user, host, real, modes);
}

void anope_cmd_kick(const char *source, const char *chan, const char *user, const char *fmt, ...)
{
	va_list args;
	char buf[BUFSIZE] = "";
	if (fmt) {
		va_start(args, fmt);
		vsnprintf(buf, BUFSIZE - 1, fmt, args);
		va_end(args);
	}
	ircdproto->cmd_kick(source, chan, user, buf);
}

void anope_cmd_notice_ops(const char *source, const char *dest, const char *fmt, ...)
{
	va_list args;
	char buf[BUFSIZE] = "";
	if (fmt) {
		va_start(args, fmt);
		vsnprintf(buf, BUFSIZE - 1, fmt, args);
		va_end(args);
	}
	ircdproto->cmd_notice_ops(source, dest, buf);
}

void anope_cmd_message(const char *source, const char *dest, const char *fmt, ...)
{
	va_list args;
	char buf[BUFSIZE] = "";
	if (fmt) {
		va_start(args, fmt);
		vsnprintf(buf, BUFSIZE - 1, fmt, args);
		va_end(args);
	}
	ircdproto->cmd_message(source, dest, buf);
}

void anope_cmd_notice(const char *source, const char *dest, const char *msg)
{
	ircdproto->cmd_notice(source, dest, msg);
}

void anope_cmd_action(const char *source, const char *dest, const char *fmt, ...)
{
	va_list args;
	char buf[BUFSIZE] = "", actionbuf[BUFSIZE] = "";
	if (fmt) {
		va_start(args, fmt);
		vsnprintf(buf, BUFSIZE - 1, fmt, args);
		va_end(args);
	}
	else return;
	if (!*buf) return;
	snprintf(actionbuf, BUFSIZE - 1, "%cACTION %s%c", 1, buf, 1);
	ircdproto->cmd_privmsg(source, dest, actionbuf);
}

void anope_cmd_privmsg(const char *source, const char *dest, const char *fmt, ...)
{
	va_list args;
	char buf[BUFSIZE] = "";
	if (fmt) {
		va_start(args, fmt);
		vsnprintf(buf, BUFSIZE - 1, fmt, args);
		va_end(args);
	}
	ircdproto->cmd_privmsg(source, dest, buf);
}

void anope_cmd_serv_notice(const char *source, const char *dest, const char *msg)
{
	ircdproto->cmd_serv_notice(source, dest, msg);
}

void anope_cmd_serv_privmsg(const char *source, const char *dest, const char *msg)
{
	ircdproto->cmd_serv_privmsg(source, dest, msg);
}

void anope_cmd_bot_chan_mode(const char *nick, const char *chan)
{
	ircdproto->cmd_bot_chan_mode(nick, chan);
}

void anope_cmd_quit(const char *source, const char *fmt, ...)
{
	va_list args;
	char buf[BUFSIZE] = "";
	if (fmt) {
		va_start(args, fmt);
		vsnprintf(buf, BUFSIZE - 1, fmt, args);
		va_end(args);
	}
	ircdproto->cmd_quit(source, buf);
}

void anope_cmd_pong(const char *servname, const char *who)
{
	ircdproto->cmd_pong(servname, who);
}

void anope_cmd_join(const char *user, const char *channel, time_t chantime)
{
	ircdproto->cmd_join(user, channel, chantime);
}

void anope_cmd_unsqline(const char *user)
{
	ircdproto->cmd_unsqline(user);
}

void anope_cmd_invite(const char *source, const char *chan, const char *nick)
{
	ircdproto->cmd_invite(source, chan, nick);
}

void anope_cmd_part(const char *nick, const char *chan, const char *fmt, ...)
{
	if (fmt) {
		va_list args;
		char buf[BUFSIZE] = "";
		va_start(args, fmt);
		vsnprintf(buf, BUFSIZE - 1, fmt, args);
		va_end(args);
		ircdproto->cmd_part(nick, chan, buf);
	}
	else ircdproto->cmd_part(nick, chan, NULL);
}

void anope_cmd_global(const char *source, const char *fmt, ...)
{
	va_list args;
	char buf[BUFSIZE] = "";
	if (fmt) {
		va_start(args, fmt);
		vsnprintf(buf, BUFSIZE - 1, fmt, args);
		va_end(args);
	}
	ircdproto->cmd_global(source, buf);
}

void anope_cmd_sqline(const char *mask, const char *reason)
{
	ircdproto->cmd_sqline(mask, reason);
}

void anope_cmd_squit(const char *servname, const char *message)
{
	ircdproto->cmd_squit(servname, message);
}

void anope_cmd_svso(const char *source, const char *nick, const char *flag)
{
	ircdproto->cmd_svso(source, nick, flag);
}

void anope_cmd_chg_nick(const char *oldnick, const char *newnick)
{
	ircdproto->cmd_chg_nick(oldnick, newnick);
}

void anope_cmd_svsnick(const char *source, const char *guest, time_t when)
{
	ircdproto->cmd_svsnick(source, guest, when);
}

void anope_cmd_vhost_on(const char *nick, const char *vIdent, const char *vhost)
{
	ircdproto->cmd_vhost_on(nick, vIdent, vhost);
}

void anope_cmd_connect()
{
	ircdproto->cmd_connect();
}

void anope_cmd_svshold(const char *nick)
{
	ircdproto->cmd_svshold(nick);
}

void anope_cmd_release_svshold(const char *nick)
{
	ircdproto->cmd_release_svshold(nick);
}

void anope_cmd_unsgline(const char *mask)
{
	ircdproto->cmd_unsgline(mask);
}

void anope_cmd_unszline(const char *mask)
{
	ircdproto->cmd_unszline(mask);
}

void anope_cmd_szline(const char *mask, const char *reason, const char *whom)
{
	ircdproto->cmd_szline(mask, reason, whom);
}

void anope_cmd_sgline(const char *mask, const char *reason)
{
	ircdproto->cmd_sgline(mask, reason);
}

void anope_cmd_unban(const char *name, const char *nick)
{
	ircdproto->cmd_unban(name, nick);
}

void anope_cmd_svsmode_chan(const char *name, const char *mode, const char *nick)
{
	ircdproto->cmd_svsmode_chan(name, mode, nick);
}

void anope_cmd_svid_umode(const char *nick, time_t ts)
{
	ircdproto->cmd_svid_umode(nick, ts);
}

void anope_cmd_nc_change(User *u)
{
	ircdproto->cmd_nc_change(u);
}

void anope_cmd_svid_umode2(User *u, const char *ts)
{
	ircdproto->cmd_svid_umode2(u, ts);
}

void anope_cmd_svid_umode3(User *u, const char *ts)
{
	ircdproto->cmd_svid_umode3(u, ts);
}

void anope_cmd_svsjoin(const char *source, const char *nick, const char *chan, const char *param)
{
	ircdproto->cmd_svsjoin(source, nick, chan, param);
}

void anope_cmd_svspart(const char *source, const char *nick, const char *chan)
{
	ircdproto->cmd_svspart(source, nick, chan);
}

void anope_cmd_swhois(const char *source, const char *who, const char *mask)
{
	ircdproto->cmd_swhois(source, who, mask);
}

void anope_cmd_eob()
{
	ircdproto->cmd_eob();
}

int anope_flood_mode_check(const char *value)
{
	return ircdproto->flood_mode_check(value);
}

void anope_cmd_jupe(const char *jserver, const char *who, const char *reason)
{
	ircdproto->cmd_jupe(jserver, who, reason);
}

int anope_valid_nick(const char *nick)
{
	return ircdproto->valid_nick(nick);
}

int anope_valid_chan(const char *chan)
{
	return ircdproto->valid_chan(chan);
}


void anope_cmd_ctcp(const char *source, const char *dest, const char *fmt, ...)
{
	va_list args;
	char buf[BUFSIZE] = "";
	if (fmt) {
		va_start(args, fmt);
		vsnprintf(buf, BUFSIZE - 1, fmt, args);
		va_end(args);
	}
	ircdproto->cmd_ctcp(source, dest, buf);
}

void anope_cmd_numeric(const char *source, int numeric, const char *dest, const char *fmt, ...)
{
	va_list args;
	char buf[BUFSIZE] = "";
	if (fmt) {
		va_start(args, fmt);
		vsnprintf(buf, BUFSIZE - 1, fmt, args);
		va_end(args);
	}
	ircdproto->cmd_numeric(source, numeric, dest, *buf ? buf : NULL);
}

/**
 * Set routines for modules to set the prefered function for dealing with things.
 **/
void pmodule_ircd_var(IRCDVar * ircdvar)
{
    ircd = ircdvar;
}

void pmodule_ircd_cap(IRCDCAPAB * cap)
{
    ircdcap = cap;
}

void pmodule_ircd_version(const char *version)
{
    version_protocol = sstrdup(version);
}

void pmodule_ircd_cbmodeinfos(CBModeInfo * modeinfos)
{
    cbmodeinfos = modeinfos;
}

void pmodule_ircd_cumodes(CUMode modes[128])
{
    int i = 0;
    for (i = 0; i < 128; i++) {
        cumodes[i] = modes[i];
    }
}

void pmodule_ircd_flood_mode_char_set(const char *mode)
{
    flood_mode_char_set = sstrdup(mode);
}

void pmodule_ircd_flood_mode_char_remove(const char *mode)
{
    flood_mode_char_remove = sstrdup(mode);
}

void pmodule_ircd_cbmodes(CBMode modes[128])
{
    int i = 0;
    for (i = 0; i < 128; i++) {
        cbmodes[i] = modes[i];
    }
}

void pmodule_ircd_cmmodes(CMMode modes[128])
{
    int i = 0;
    for (i = 0; i < 128; i++) {
        cmmodes[i] = modes[i];
    }
}

void pmodule_ircd_csmodes(char mode[128])
{
    int i = 0;
    for (i = 0; i < 128; i++) {
        csmodes[i] = mode[i];
    }
}

void pmodule_ircd_useTSMode(int use)
{
    UseTSMODE = use;
}

/** mode stuff */

void pmodule_invis_umode(int mode)
{
    ircd_modes.user_invis = mode;
}

void pmodule_oper_umode(int mode)
{
    ircd_modes.user_oper = mode;
}

void pmodule_invite_cmode(int mode)
{
    ircd_modes.chan_invite = mode;
}

void pmodule_secret_cmode(int mode)
{
    ircd_modes.chan_secret = mode;
}

void pmodule_private_cmode(int mode)
{
    ircd_modes.chan_private = mode;
}

void pmodule_key_mode(int mode)
{
    ircd_modes.chan_key = mode;
}

void pmodule_limit_mode(int mode)
{
    ircd_modes.chan_limit = mode;
}

int anope_get_invis_mode()
{
    return ircd_modes.user_invis;
}

int anope_get_oper_mode()
{
    return ircd_modes.user_oper;
}

int anope_get_invite_mode()
{
    return ircd_modes.chan_invite;
}

int anope_get_secret_mode()
{
    return ircd_modes.chan_secret;
}

int anope_get_private_mode()
{
    return ircd_modes.chan_private;
}

int anope_get_key_mode()
{
    return ircd_modes.chan_key;
}

int anope_get_limit_mode()
{
    return ircd_modes.chan_limit;
}
