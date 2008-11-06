/* ChanServ core functions
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
/*************************************************************************/

#include "module.h"

int do_sendpass(User * u);
void myChanServHelp(User * u);

class CSSendPass : public Module
{
 public:
	CSSendPass(const std::string &creator) : Module(creator)
	{

		Command *c;

		moduleAddAuthor("Anope");
		moduleAddVersion("$Id$");
		moduleSetType(CORE);

		c = createCommand("SENDPASS", do_sendpass, NULL, CHAN_HELP_SENDPASS,
		-1, -1, -1, -1);
		moduleAddCommand(CHANSERV, c, MOD_UNIQUE);

		moduleSetChanHelp(myChanServHelp);

		if (!UseMail)
		{
			throw ModuleException("sendpass may not be loaded if UseMail is loaded");
		}
	}
};



/**
 * Add the help response to anopes /cs help output.
 * @param u The user who is requesting help
 **/
void myChanServHelp(User * u)
{
    notice_lang(s_ChanServ, u, CHAN_HELP_CMD_SENDPASS);
}

/**
 * The /cs sendpass command.
 * @param u The user who issued the command
 * @param MOD_CONT to continue processing other modules, MOD_STOP to stop processing.
 **/
int do_sendpass(User * u)
{

    char *chan = strtok(NULL, " ");
    ChannelInfo *ci;
    NickCore *founder;

    if (!chan) {
        syntax_error(s_ChanServ, u, "SENDPASS", CHAN_SENDPASS_SYNTAX);
    } else if (RestrictMail && !is_services_oper(u)) {
        notice_lang(s_ChanServ, u, PERMISSION_DENIED);
    } else if (!(ci = cs_findchan(chan)) || !(founder = ci->founder)) {
        notice_lang(s_ChanServ, u, CHAN_X_NOT_REGISTERED, chan);
    } else if (ci->flags & CI_VERBOTEN) {
        notice_lang(s_ChanServ, u, CHAN_X_FORBIDDEN, chan);
    } else {
        char buf[BUFSIZE];
        char tmp_pass[PASSMAX];
        if(enc_decrypt(ci->founderpass,tmp_pass,PASSMAX - 1)==1) {
            MailInfo *mail;

            snprintf(buf, sizeof(buf),
                     getstring2(founder, CHAN_SENDPASS_SUBJECT), ci->name);
            mail = MailBegin(u, founder, buf, s_ChanServ);
            if (!mail)
                return MOD_CONT;

            fprintf(mail->pipe, getstring2(founder, CHAN_SENDPASS_HEAD));
            fprintf(mail->pipe, "\n\n");
            fprintf(mail->pipe, getstring2(founder, CHAN_SENDPASS_LINE_1),
                    ci->name);
            fprintf(mail->pipe, "\n\n");
            fprintf(mail->pipe, getstring2(founder, CHAN_SENDPASS_LINE_2),
                    tmp_pass);
            fprintf(mail->pipe, "\n\n");
            fprintf(mail->pipe, getstring2(founder, CHAN_SENDPASS_LINE_3));
            fprintf(mail->pipe, "\n\n");
            fprintf(mail->pipe, getstring2(founder, CHAN_SENDPASS_LINE_4));
            fprintf(mail->pipe, "\n\n");
            fprintf(mail->pipe, getstring2(founder, CHAN_SENDPASS_LINE_5),
                    NetworkName);
            fprintf(mail->pipe, "\n.\n");
 
            MailEnd(mail);

            alog("%s: %s!%s@%s used SENDPASS on %s", s_ChanServ, u->nick,
                 u->username, u->host, chan);
            notice_lang(s_ChanServ, u, CHAN_SENDPASS_OK, chan);
        } else {
            notice_lang(s_ChanServ, u, CHAN_SENDPASS_UNAVAILABLE);
        }
    }
    return MOD_CONT;
}

MODULE_INIT("cs_sendpass", CSSendPass)
